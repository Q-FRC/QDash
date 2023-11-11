#include "TopicStore.h"

#include "Globals.h"

QHash<std::string, nt::NetworkTableEntry *> TopicStore::topicEntryMap{};
QMultiHash<std::string, QObject *> TopicStore::topicSubscriberMap{};

TopicStore::TopicStore()
{
    throw std::runtime_error("TopicStore is a static class.");
}

nt::NetworkTableEntry *TopicStore::subscribe(std::string ntTopic, QObject *receiver) {
    if (!topicEntryMap.contains(ntTopic)) {
        nt::NetworkTableEntry *entry =
            new nt::NetworkTableEntry(nt::GetEntry(Globals::inst.GetHandle(), ntTopic));

        topicEntryMap.insert(ntTopic, entry);
    }

    if (!topicSubscriberMap.contains(ntTopic, receiver)) topicSubscriberMap.insert(ntTopic, receiver);

    return topicEntryMap.value(ntTopic);
}

void TopicStore::unsubscribe(std::string ntTopic, QObject *receiver) {
    if (!topicEntryMap.contains(ntTopic)) return;
    if (!topicSubscriberMap.contains(ntTopic, receiver)) return;

    topicSubscriberMap.remove(ntTopic, receiver);

    if (!topicSubscriberMap.contains(ntTopic)) {
        nt::NetworkTableEntry *entry = topicEntryMap.value(ntTopic);

        entry->Unpublish();

        topicEntryMap.remove(ntTopic);
    }
}

void TopicStore::unsubscribe(nt::NetworkTableEntry *entry, QObject *subscriber) {
    unsubscribe(entry->GetName(), subscriber);
}
