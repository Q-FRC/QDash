// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "NT/TopicStore.h"

#include "Misc/Globals.h"
#include "networktables/NetworkTableEntry.h"

#include <QGuiApplication>
#include <QThread>
#include <QTimer>
#include <QVariant>

TopicStore::TopicStore(QQmlEngine* engine, Logger* logs, QObject* parent)
    : QObject(parent), m_logs(logs), m_engine(engine) {}

bool Listener::operator==(const Listener& other) const {
    return (other.topic() == m_topic);
}

Listener::Listener(QQmlEngine* engine, QString topic, QObject* parent)
    : QObject(parent), m_topic(topic), m_engine(engine) {
    m_topic = topic;
    m_entry = Globals::inst.GetEntry(topic.toStdString());

    bindHandle();
}

QString Listener::topic() const {
    return m_topic;
}

void Listener::addListener(const QJSValue& func) {
    m_funcs.emplaceBack(func);

    updateEvent();
}

bool Listener::rmListener(const QJSValue& func) {
    // QJSValue lacks an operator== :(
    for (size_t i = 0; i < m_funcs.size(); ++i) {
        const QJSValue& f = m_funcs.at(i);
        if (f.strictlyEquals(func)) {
            m_funcs.removeAt(i);
            return true;
        }
    }

    return false;
}

bool Listener::empty() {
    return m_funcs.empty();
}

void Listener::updateEvent(const nt::Event& event) {
    QVariant value;
    if (!event.Is(nt::EventFlags::kValueAll))
        value = getValue();
    else // TODO(crueter): Evaluate perf
        value = TopicStore::toVariant(event.GetValueEventData()->value);

    update(value);
}

void Listener::update(const QVariant& value) {
    if (value.isNull() || !value.isValid())
        return;

    // Ensure the call is run in the QSG thread
    // You can't use invokeMethod on a QJSValue so we just wrap it in a timer here.
    QTimer* timer = new QTimer();
    timer->moveToThread(qApp->thread());
    timer->setSingleShot(true);

    QMetaObject::Connection *conn = new QMetaObject::Connection;
    *conn = connect(timer, &QTimer::timeout, this, [timer, this, value, conn]() {
        for (const QJSValue& func : std::as_const(m_funcs)) {
            func.call({m_engine->toScriptValue(value)});
        }

        timer->deleteLater();

        disconnect(*conn);
        delete conn;
    });

    QMetaObject::invokeMethod(timer, "start", Qt::QueuedConnection, Q_ARG(int, 0));
}

void Listener::unpublish() {
    m_entry.Unpublish();
    Globals::inst.RemoveListener(m_handle);
}

void Listener::setValue(const QVariant& value) {
    m_entry.SetValue(TopicStore::toValue(value));
}

QVariant Listener::getValue() {
    return TopicStore::toVariant(m_entry.GetValue());
}

void Listener::bindHandle() {
    m_callback = [this](const nt::Event& event) {
        QMetaObject::invokeMethod(this, "updateEvent", Qt::DirectConnection,
                                  Q_ARG(nt::Event, event));
    };

    m_handle = Globals::inst.AddListener(m_entry, nt::EventFlags::kValueAll, m_callback);
}

void TopicStore::subscribe(QString topic, const QJSValue& func) {
    if (topic == "")
        return;

    Listener *listener = entry(topic);

    if (!listener) {
        // TODO: fmt
        m_logs->debug("TopicStore", "Creating new listener for topic " + topic);

        listener = new Listener(m_engine, topic, this);
        Listeners.append(listener);
    }

    listener->addListener(func);

    m_logs->info("TopicStore", "Subscribed to topic " + topic);
}

void TopicStore::unsubscribe(QString topic, const QJSValue& func) {
    Listener *l = entry(topic);
    if (!l)
        return;

    l->rmListener(func);

    if (l->empty()) {
        m_logs->debug("TopicStore", "Destructing listener for topic " + topic);
        l->unpublish();
        Listeners.removeAll(l);
        l->deleteLater();
    }

    m_logs->debug("TopicStore", "Unsubscribed from topic " + topic);
}

void TopicStore::subscribeOneShot(QString topic, std::function<void(QVariant)> callback) {
    if (topic.isEmpty() || !callback) return;

    nt::NetworkTableEntry entry = Globals::inst.GetEntry(topic.toStdString());

    // The lambda needs to reference its own handle in order to destruct it.
    // Shared pointer is used because otherwise you get weird thread contention stuff,
    // and also we can't just delete an integer :(
    auto handle = std::make_shared<NT_Listener>(0);

    auto ntCallback = [this, callback, handle, entry](const nt::Event &event) mutable {
        QVariant value = toVariant(event.GetValueEventData()->value);
        callback(value);

        entry.Unpublish();
        Globals::inst.RemoveListener(*handle);
    };

    *handle = Globals::inst.AddListener(entry, nt::EventFlags::kValueAll, ntCallback);

    m_logs->debug("TopicStore", "One-shot subscription requested to topic " + topic);
}

QVariant TopicStore::getValue(QString topic) {
    Listener* l = entry(topic);
    if (l)
        return l->getValue();

    return QVariant{};
}

void TopicStore::setValue(QString topic, const QVariant& value) {
    Listener* l = entry(topic);
    if (l)
        l->setValue(value);
}

void TopicStore::forceUpdate(const QString& topic) {
    m_logs->debug("TopicStore", "Force-updating topic " + topic);

    Listener* l = entry(topic);
    if (l)
        l->updateEvent();
}

QString TopicStore::typeString(QString topic) {
    nt::NetworkTableEntry entry = Globals::inst.GetEntry(topic.toStdString());
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
    case nt::NetworkTableType::kBooleanArray:
        return "reef";
    case nt::NetworkTableType::kStringArray:
        return "errors";
    default:
        return "";
    }
}

QVariant TopicStore::toVariant(const nt::Value& value) {
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
        for (const int i : a)
            newList << i;

        v = QVariant::fromValue(newList);
    } else if (value.IsStringArray()) {
        const std::span<const std::string> a = value.GetStringArray();
        QStringList newList;
        for (const std::string& s : a)
            newList << QString::fromStdString(s);

        v = QVariant::fromValue(newList);
    } else if (value.IsDoubleArray()) {
        const std::span<const double> a = value.GetDoubleArray();
        QList<double> newList;
        for (const double d : a)
            newList << d;

        v = QVariant::fromValue(newList);
    } else if (value.IsIntegerArray()) {
        const std::span<const int64_t> a = value.GetIntegerArray();
        QList<int64_t> newList;
        for (const size_t i : a)
            newList << i;

        v = QVariant::fromValue(newList);
    }

    return v;
}

nt::Value TopicStore::toValue(const QVariant& value) {
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
        for (const QString& s : value.toStringList()) {
            v.emplace_back(s.toStdString());
        }

        return nt::Value::MakeStringArray(v);
    }
    default:
        break;
    }

    if (value.typeId() == qMetaTypeId<QList<bool>>()) {
        std::vector<int> v;
        for (const QVariant& b : value.toList()) {
            v.emplace_back(b.toBool());
        }

        return nt::Value::MakeBooleanArray(v);
    }

end:
    return nt::Value();
}

void TopicStore::connect(bool connected) {
    emit this->connected(connected);
}

bool TopicStore::hasEntry(QString topic) {
    return !entry(topic);
}

Listener* TopicStore::entry(QString topic) {
    for (Listener* listener : std::as_const(Listeners)) {
        if (listener->topic() == topic) {
            return listener;
        }
    }

    return nullptr;
}
