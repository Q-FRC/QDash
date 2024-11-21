#include "stores/TopicStore.h"

#include "Globals.h"

#include <QVariant>

TopicStore::TopicStore(QObject *parent)
    : QObject(parent)
{
}

void TopicStore::connect(bool connected)
{
    emit this->connected(connected);
}

bool TopicStore::hasEntry(QString topic) {
    for (const Listener &listener : Listeners) {
        if (topic == listener.topic) {
            return true;
        }
    }
    return false;
}

Listener TopicStore::entry(QString topic) {
    for (Listener listener : Listeners) {
        if (listener.topic == topic) {
            return listener;
        }
    }

    Listener l;
    l.isNull = true;
    return l;
}

Listener TopicStore::changeNumSubscribed(QString topic, int changeBy)
{
    for (Listener listener : Listeners) {
        if (listener.topic == topic) {
            listener.numSubscribed += changeBy;
            return listener;
        }
    }

    Listener l;
    l.isNull = true;
    return l;
}

QVariant TopicStore::toVariant(const nt::Value &value)
{
    QVariant v;

    if (!value.IsValid()) return v;

    if (value.IsBoolean()) v = value.GetBoolean();
    else if (value.IsString()) v = QString::fromStdString(std::string(value.GetString()));
    else if (value.IsDouble()) v = value.GetDouble();
    else if (value.IsFloat()) v = value.GetFloat();
    else if (value.IsInteger()) v = QVariant::fromValue(value.GetInteger());

    else if (value.IsBooleanArray()) {
        const std::span<const int> a = value.GetBooleanArray();
        QList<int> newList;
        for (const int i : a) {
            newList << i;
        }
        v = QVariant::fromValue(newList);
    }
    else if (value.IsStringArray()) {
        const std::span<const std::string> a = value.GetStringArray();
        QStringList newList;
        for (const std::string &s : a) {
            newList << QString::fromStdString(s);
        }
        v = QVariant::fromValue(newList);
    }
    else if (value.IsDoubleArray()) {
        const std::span<const double> a = value.GetDoubleArray();
        QList<double> newList;
        for (const double d : a) {
            newList << d;
        }
        v = QVariant::fromValue(newList);
    }
    else if (value.IsIntegerArray()) {
        const std::span<const int64_t> a = value.GetIntegerArray();
        QList<int64_t> newList;
        for (const size_t i : a) {
            newList << i;
        }
        v = QVariant::fromValue(newList);
    }

    return v;
}

nt::Value TopicStore::toValue(const QVariant &value)
{
    if (!value.isValid()) goto end;

    switch (value.typeId()) {
    case QMetaType::Type::QString:
        return nt::Value::MakeString(std::string_view{value.toString().toStdString()});
    case QMetaType::Type::QStringList: {
        std::vector<std::string> v;
        for (const QString &s : value.toStringList()) {
            v.emplace_back(s.toStdString());
        }

        return nt::Value::MakeStringArray(v);
    } case QMetaType::Type::Bool:
        return nt::Value::MakeBoolean(value.toBool());
    case QMetaType::Type::Double:
        return nt::Value::MakeDouble(value.toDouble());
    case QMetaType::Type::Float:
        return nt::Value::MakeFloat(value.toFloat());
    case QMetaType::Type::Int:
        return nt::Value::MakeInteger(value.toInt());
    }

end:
    return nt::Value();
}

bool Listener::operator==(const Listener &other) const {
    return (other.topic == this->topic) &&
           (other.isNull == this->isNull);
}

void TopicStore::subscribe(QString ntTopic) {
    Listener listener;

    listener = changeNumSubscribed(ntTopic);
    if (listener.isNull) {
        listener = {
            ntTopic,
            0,
            nt::ListenerCallback(),
            1,
            false
        };

        nt::NetworkTableEntry entry = Globals::inst.GetEntry(ntTopic.toStdString());

        topicEntryMap.insert(ntTopic, entry);

        nt::ListenerCallback updateWidget = [entry, ntTopic, this](const nt::Event &event = nt::Event()) {
            nt::Value value;
            if (!event.Is(nt::EventFlags::kValueAll)) {
                value = entry.GetValue();
            } else {
                value = event.GetValueEventData()->value;
            }

            if (value.IsValid()) {
                emit topicUpdate(ntTopic, toVariant(value));
            }
        };

        NT_Listener handle = Globals::inst.AddListener(entry, nt::EventFlags::kValueAll, updateWidget);

        listener.listenerHandle = handle;
        listener.callback = updateWidget;

        Listeners.append(listener);
    } else {
        listener.callback(nt::Event());
    }
}

void TopicStore::unsubscribe(QString ntTopic) {
    if (!hasEntry(ntTopic)) return;

    Listener listener = changeNumSubscribed(ntTopic, -1);

    if (listener.numSubscribed <= 0) {
        topicEntryMap.value(ntTopic).Unpublish();
        topicEntryMap.remove(ntTopic);
        Listeners.removeAll(listener);
        Globals::inst.RemoveListener(listener.listenerHandle);
    }
}

double TopicStore::getDoubleFromEntry(nt::NetworkTableEntry entry) {
    nt::Value value = entry.GetValue();

    if (value.IsBoolean()) {
        return (double) entry.GetBoolean(0);
    } else if (value.IsDouble()) {
        return entry.GetDouble(0.);
    } else if (value.IsInteger()) {
        return (double) entry.GetInteger(0);
    }

    return 0.;
}

QVariant TopicStore::getValue(QString topic) {
    Listener l = entry(topic);
    if (l.isNull) return QVariant{};

    nt::NetworkTableEntry entry = topicEntryMap.value(topic);
    QVariant v = toVariant(entry.GetValue());
    return v;
}

void TopicStore::setValue(QString topic, const QVariant &value)
{
    Listener l = entry(topic);
    if (l.isNull) return;

    nt::NetworkTableEntry entry = topicEntryMap.value(topic);
    entry.SetValue(toValue(value));

}

QString TopicStore::typeString(QString topic)
{
    nt::NetworkTableEntry entry = Globals::inst.GetEntry(topic.toStdString());
    nt::NetworkTableType type = entry.GetType();

    switch (type) {
    case nt::NetworkTableType::kBoolean: return "bool";
    case nt::NetworkTableType::kDouble: return "double";
    case nt::NetworkTableType::kFloat: return "double";
    case nt::NetworkTableType::kString: return "string";
    case nt::NetworkTableType::kInteger: return "int";
    case nt::NetworkTableType::kStringArray: return "errors";
    default:
        return "";
    }
}
