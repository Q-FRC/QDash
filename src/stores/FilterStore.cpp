#include "stores/FilterStore.h"

QMap<std::string, TopicTypes> FilterStore::m_sendableTypeMap{};
QMap<nt::NetworkTableType, TopicTypes> FilterStore::m_ntTypeMap{};

QMap<QString, TopicTypes> FilterStore::FilteredTopics{};

FilterStore::FilterStore()
{
    throw std::runtime_error("FilterStore is a static class.");
}

void FilterStore::registerSendable(std::string typeString, TopicTypes topicType) {
    m_sendableTypeMap.insert(typeString, topicType);
}

void FilterStore::registerNTType(nt::NetworkTableType ntType, TopicTypes topicType) {
    m_ntTypeMap.insert(ntType, topicType);
}

std::optional<TopicTypes> FilterStore::sendableTypeForTypeString(std::string typeString) {
    return m_sendableTypeMap.contains(typeString) ? std::optional<TopicTypes>(m_sendableTypeMap.value(typeString)) : std::nullopt;
}

std::optional<TopicTypes> FilterStore::topicTypeForNTType(nt::NetworkTableType ntType) {
    return m_ntTypeMap.contains(ntType) ? std::optional<TopicTypes>(m_ntTypeMap.value(ntType)) : std::nullopt;
}

void FilterStore::filterTopics() {
    for (const QString &topic : Globals::ntTopics) {
        sortTopic(topic);
    }

    // delete unused topics
    const QStringList &topics = FilteredTopics.keys();

    for (const QString &topic : topics) {
        QStringList split = topic.split('/');
        QString supertable = split.sliced(0, split.length() - 1).join('/');

        // account for sendables
        if (
            (!Globals::ntTopics.contains(topic) &&
             !Globals::ntTopics.contains(topic + "/.type")) ||
            topics.contains(supertable)) {
            FilteredTopics.remove(topic);
        }
    }
}

void FilterStore::sortTopic(QString topic) {
    std::string topicName = topic.toStdString();
    TopicTypes topicType;

    nt::NetworkTableEntry entry = Globals::inst.GetEntry(topicName);

    QStringList split = QString::fromStdString(topicName).split('/');
    QString supertable = split.sliced(0, split.length() - 1).join('/');

    nt::NetworkTableEntry typeEntry;
    std::shared_ptr<nt::NetworkTable> table = Globals::inst.GetTable(supertable.toStdString());

    if (!supertable.isEmpty()) {
        typeEntry = table->GetEntry(".type");
    } else {
        typeEntry = Globals::inst.GetEntry("");
    }

    std::string value = typeEntry.GetString("");

    std::optional<TopicTypes> sendableType = sendableTypeForTypeString(value);
    std::optional<TopicTypes> ntType = topicTypeForNTType(entry.GetType());

    if (sendableType != std::nullopt) {
        FilteredTopics.insert(supertable, sendableType.value());
    } else if (ntType != std::nullopt) {
        FilteredTopics.insert(topic, ntType.value());
    } else {
        FilteredTopics.insert(topic, TopicTypes::String);
    }

    entry.Unpublish();
    typeEntry.Unpublish();
}
