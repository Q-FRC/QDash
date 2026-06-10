// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "BuildConfig/BuildConfig.h"
#include "Services/TopicStore.h"

TopicStore::TopicStore(QQmlEngine *engine, Logger *logs, QObject *parent)
    : QObject(parent), m_logs(logs), m_engine(engine),
      m_instance{nt::NetworkTableInstance::GetDefault()}
{
    m_instance.StartClient4(BuildConfig.APPLICATION_NAME.toStdString());

    // Connections //
    m_instance.AddConnectionListener(true, [this](const nt::Event &event) {
        bool connected = event.Is(nt::EventFlags::kConnected);
        QString remoteIP = QString::fromStdString(event.GetConnectionInfo()->remote_ip);

        QMetaObject::invokeMethod(this, [this, remoteIP, connected] {
            emit connectedStateChanged(connected);

            if (connected) {
                m_logs->info("NT", "Client connected to " + remoteIP);
                emit this->connected(remoteIP);
            } else {
                m_logs->info("NT", "Client disconnected");
                emit this->disconnected();
            }
        });
    });

    // Topic Publishes //
    m_instance.AddListener({{""}}, nt::EventFlags::kPublish, [this](const nt::Event &event) {
        std::string topicName(event.GetTopicInfo()->name);
        QMetaObject::invokeMethod(this, [this, topicName]() {
            m_logs->debug("NT",
                          "Received topic announcement for " + QString::fromStdString(topicName));
            emit topicPublished(topicName);
        });
    });

    // Topic Unpublishes //
    m_instance.AddListener({{""}}, nt::EventFlags::kUnpublish, [this](const nt::Event &event) {
        std::string topicName(event.GetTopicInfo()->name);
        QMetaObject::invokeMethod(this, [this, topicName]() {
            m_logs->debug("NT", "Received topic unpublish event for " +
                                    QString::fromStdString(topicName));
            emit topicUnpublished(topicName);
        });
    });
}

bool Listener::operator==(const Listener &other) const
{
    return (other.topic() == m_topic);
}

Listener::Listener(QQmlEngine *engine, nt::NetworkTableInstance instance, QString topic,
                   QObject *parent)
    : QObject(parent), m_topic(topic), m_engine(engine), m_instance{instance}
{
    m_topic = topic;
    m_entry = m_instance.GetEntry(topic.toStdString());

    bindHandle();
}

QString Listener::topic() const
{
    return m_topic;
}

void Listener::addListener(const QJSValue &func)
{
    m_funcs.emplaceBack(func);

    updateEvent();
}

bool Listener::rmListener(const QJSValue &func)
{
    // QJSValue lacks an operator== :(
    for (qsizetype i = 0; i < m_funcs.size(); ++i) {
        const QJSValue &f = m_funcs.at(i);
        if (f.strictlyEquals(func)) {
            m_funcs.removeAt(i);
            return true;
        }
    }

    return false;
}

bool Listener::empty()
{
    return m_funcs.empty();
}

void Listener::updateEvent(const nt::Event &event)
{
    QVariant value;
    if (!event.Is(nt::EventFlags::kValueAll))
        value = getValue();
    else // TODO(crueter): Evaluate perf
        value = TopicStore::toVariant(event.GetValueEventData()->value);

    update(value);
}

void Listener::update(const QVariant &value)
{
    if (value.isNull() || !value.isValid())
        return;

    for (const QJSValue &func : std::as_const(m_funcs)) {
        func.call({m_engine->toScriptValue(value)});
    }
}

void Listener::unpublish()
{
    m_entry.Unpublish();
    m_instance.RemoveListener(m_handle);
}

void Listener::setValue(const QVariant &value)
{
    m_entry.SetValue(TopicStore::toValue(value));
}

QVariant Listener::getValue()
{
    return TopicStore::toVariant(m_entry.GetValue());
}

void Listener::bindHandle()
{
    m_callback = [this](const nt::Event &event) {
        // queue listener invocation so it runs on QSG thread
        QVariant value = event.Is(nt::EventFlags::kValueAll)
                             ? TopicStore::toVariant(event.GetValueEventData()->value)
                             : getValue();

        QMetaObject::invokeMethod(
            this, [this, v = std::move(value)]() { update(v); }, Qt::QueuedConnection);
    };

    m_handle = m_instance.AddListener(m_entry, nt::EventFlags::kValueAll, m_callback);
}

void TopicStore::subscribe(const QString &topic, const QJSValue &func)
{
    if (topic == "")
        return;

    Listener *listener = entry(topic);

    if (!listener) {
        // TODO: fmt
        m_logs->debug("TopicStore", "Creating new listener for topic " + topic);

        listener = new Listener(m_engine, m_instance, topic, this);
        m_listeners.insert(topic, listener);
    }

    listener->addListener(func);

    m_logs->info("TopicStore", "Subscribed to topic " + topic);
}

void TopicStore::unsubscribe(const QString &topic, const QJSValue &func)
{
    Listener *l = entry(topic);
    if (!l)
        return;

    l->rmListener(func);

    if (l->empty()) {
        m_logs->debug("TopicStore", "Destructing listener for topic " + topic);
        l->unpublish();
        m_listeners.remove(topic);
        l->deleteLater();
    }

    m_logs->debug("TopicStore", "Unsubscribed from topic " + topic);
}

void TopicStore::subscribeOneShot(const QString &topic, std::function<void(QVariant)> callback)
{
    if (topic.isEmpty() || !callback)
        return;

    nt::NetworkTableEntry entry = m_instance.GetEntry(topic.toStdString());

    // The lambda needs to reference its own handle in order to destruct it.
    // Shared pointer is used because otherwise you get weird thread contention stuff,
    // and also we can't just delete an integer.
    auto handle = std::make_shared<NT_Listener>(0);

    auto ntCallback = [callback, handle, entry, this](const nt::Event &event) mutable {
        QVariant value = toVariant(event.GetValueEventData()->value);
        callback(value);

        entry.Unpublish();
        m_instance.RemoveListener(*handle);
    };

    *handle = m_instance.AddListener(entry, nt::EventFlags::kValueAll, ntCallback);

    m_logs->debug("TopicStore", "One-shot subscription requested to topic " + topic);
}

QVariant TopicStore::getValue(const QString &topic)
{
    Listener *l = entry(topic);
    if (l)
        return l->getValue();

    return QVariant{};
}

void TopicStore::setValue(const QString &topic, const QVariant &value)
{
    Listener *l = entry(topic);
    if (l)
        l->setValue(value);
}

void TopicStore::forceUpdate(const QString &topic)
{
    m_logs->debug("TopicStore", "Force-updating topic " + topic);

    Listener *l = entry(topic);
    if (l)
        l->updateEvent();
}

QString TopicStore::typeString(const QString &topic)
{
    nt::NetworkTableEntry entry = m_instance.GetEntry(topic.toStdString());
    nt::NetworkTableType type = entry.GetType();

    switch (type) {
    case nt::NetworkTableType::kBoolean:
        return "bool";
    case nt::NetworkTableType::kDouble:
        return "double";
    case nt::NetworkTableType::kFloat:
        return "double";
    case nt::NetworkTableType::kString:
        return "string";
    case nt::NetworkTableType::kInteger:
        return "int";
    // case nt::NetworkTableType::kBooleanArray:
    //     return "reef";
    // case nt::NetworkTableType::kStringArray:
    //     return "errors";
    default:
        return "";
    }
}

QVariant TopicStore::toVariant(const nt::Value &value)
{
    QVariant v;

    if (!value.IsValid())
        return v;

    if (value.IsBoolean())
        v = value.GetBoolean();
    else if (value.IsString())
        v = QString::fromStdString(std::string(value.GetString()));
    else if (value.IsDouble())
        v = value.GetDouble();
    else if (value.IsFloat())
        v = value.GetFloat();
    else if (value.IsInteger())
        v = QVariant::fromValue(value.GetInteger());

    else if (value.IsBooleanArray()) {
        const std::span<const int> a = value.GetBooleanArray();
        QList<int> newList;
        newList.reserve(a.size());
        for (const int i : a)
            newList << i;

        v = QVariant::fromValue(newList);
    } else if (value.IsStringArray()) {
        const std::span<const std::string> a = value.GetStringArray();
        QStringList newList;
        newList.reserve(a.size());
        for (const std::string &s : a)
            newList << QString::fromStdString(s);

        v = QVariant::fromValue(newList);
    } else if (value.IsDoubleArray()) {
        const std::span<const double> a = value.GetDoubleArray();
        QList<double> newList;
        newList.reserve(a.size());
        for (const double d : a)
            newList << d;

        v = QVariant::fromValue(newList);
    } else if (value.IsIntegerArray()) {
        const std::span<const int64_t> a = value.GetIntegerArray();
        QList<int64_t> newList;
        newList.reserve(a.size());
        for (const int64_t i : a)
            newList << i;

        v = QVariant::fromValue(newList);
    }

    return v;
}

nt::Value TopicStore::toValue(const QVariant &value)
{
    if (!value.isValid())
        goto end;

    switch (value.typeId()) {
    case QMetaType::Type::QString:
        return nt::Value::MakeString(std::string_view{value.toString().toStdString()});
    case QMetaType::Type::Bool:
        return nt::Value::MakeBoolean(value.toBool());
    case QMetaType::Type::Double:
        return nt::Value::MakeDouble(value.toDouble());
    case QMetaType::Type::Float:
        return nt::Value::MakeFloat(value.toFloat());
    case QMetaType::Type::Int:
        return nt::Value::MakeInteger(value.toInt());
    case QMetaType::Type::QStringList: {
        std::vector<std::string> v;
        for (const QString &s : value.toStringList()) {
            v.emplace_back(s.toStdString());
        }

        return nt::Value::MakeStringArray(v);
    }
    default:
        break;
    }

    if (value.typeId() == qMetaTypeId<QList<bool>>()) {
        std::vector<int> v;
        for (const QVariant &b : value.toList()) {
            v.emplace_back(b.toBool());
        }

        return nt::Value::MakeBooleanArray(v);
    }

end:
    return nt::Value();
}

Listener *TopicStore::entry(const QString &topic)
{
    return m_listeners.value(topic, nullptr);
}

// NT Interface //
nt::NetworkTableEntry TopicStore::getRawEntry(const std::string_view &path)
{
    return m_instance.GetEntry(path);
}

std::vector<nt::ConnectionInfo> TopicStore::getConnections() const
{
    return m_instance.GetConnections();
}

void TopicStore::setServer(const std::string &server)
{
    m_instance.SetServer(server.c_str());
}

void TopicStore::setServerTeam(const int team)
{
    m_instance.SetServerTeam(team);
}

void TopicStore::startDSClient()
{
    m_instance.StartDSClient();
}

void TopicStore::disconnectServer()
{
    m_instance.Disconnect();
}
