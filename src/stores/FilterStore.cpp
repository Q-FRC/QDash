#include "stores/FilterStore.h"

#include "Globals.h"

QMap<std::string, TopicTypes> FilterStore::m_sendableTypeMap{};
QMap<nt::NetworkTableType, TopicTypes> FilterStore::m_ntTypeMap{};

QList<Globals::Topic> FilterStore::FilteredTopics{};
QList<Globals::Topic> FilterStore::UnfilteredTopics{};

FilterStore::FilterStore()
{
    throw std::runtime_error("FilterStore is a static class.");
}

void FilterStore::registerSendable(std::string typeString, TopicTypes topicType) {
    m_sendableTypeMap.insert(typeString, topicType);
    Globals::topicTypeDisplayNames.insert(topicType, QString::fromStdString(typeString));
}

void FilterStore::registerNTType(nt::NetworkTableType ntType, TopicTypes topicType, const QString &displayName) {
    m_ntTypeMap.insert(ntType, topicType);
    Globals::topicTypeDisplayNames.insert(topicType, displayName);
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
    for (const Globals::Topic &topic : FilteredTopics) {
        if (!FilteredTopics.contains(topic)) return;

        QStringList split = topic.name.split('/');
        Globals::Topic supertable(topic);
        supertable.name = split.sliced(0, split.length() - 1).join('/');

        // account for sendables
        if (FilteredTopics.contains(supertable)) {
            FilteredTopics.removeAll(topic);
        }

        if (
            (!Globals::ntTopics.contains(topic.name) &&
             !Globals::ntTopics.contains(topic.name + "/.type"))) {
            FilteredTopics.removeAll(topic);
            UnfilteredTopics.removeAll(topic);
        }
    }
}

void FilterStore::sortTopic(QString topic) {
    std::string topicName = topic.toStdString();
    TopicTypes topicType;

    nt::NetworkTableEntry entry = Globals::inst.GetEntry(topicName);

    if (!entry.GetValue().IsValid()) {
        return;
    }

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
    if (ntType == std::nullopt) ntType = {TopicTypes::String};

    Globals::Topic ntTopic{topic, ntType.value()};

    if (sendableType != std::nullopt) {
        Globals::Topic sendableTopic{supertable, sendableType.value()};
        if (!FilteredTopics.contains(sendableTopic)) FilteredTopics.append(sendableTopic);
    } else {
        if (!FilteredTopics.contains(ntTopic)) FilteredTopics.append(ntTopic);
    }

    if (!UnfilteredTopics.contains(ntTopic)) UnfilteredTopics.append(ntTopic);

    entry.Unpublish();
    typeEntry.Unpublish();
}
