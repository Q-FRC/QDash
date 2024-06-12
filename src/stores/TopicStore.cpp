#include "stores/TopicStore.h"
#include "widgets/BaseWidget.h"

#include "Globals.h"

QHash<std::string, nt::NetworkTableEntry *> TopicStore::topicEntryMap{};
QMultiHash<std::string, BaseWidget *> TopicStore::topicSubscriberMap{};
QHash<std::pair<std::string, BaseWidget *>, NT_Listener> TopicStore::topicListenerMap{};

TopicStore::TopicStore()
{
    throw std::runtime_error("TopicStore is a static class.");
}

nt::NetworkTableEntry *TopicStore::subscribe(std::string ntTopic, BaseWidget *subscriber) {
    if (!topicEntryMap.contains(ntTopic)) {
        nt::NetworkTableEntry *entry =
            new nt::NetworkTableEntry(nt::GetEntry(Globals::inst.GetHandle(), ntTopic));

        topicEntryMap.insert(ntTopic, entry);
    }

    if (!topicSubscriberMap.contains(ntTopic, subscriber)) topicSubscriberMap.insert(ntTopic, subscriber);

    nt::NetworkTableEntry *entry = topicEntryMap.value(ntTopic);

    nt::ListenerCallback updateWidget = [entry, subscriber](const nt::Event &event = nt::Event()) {
        nt::Value value = entry->GetValue();

        // ensure thread-safety
        if (value.IsValid())
            QMetaObject::invokeMethod(subscriber, [subscriber, value] {
                if (subscriber->ready()) {
                    subscriber->setValue(value);
                    subscriber->update();
                } else {
                    QObject *receiver = new QObject;
                    connect(subscriber, &BaseWidget::isReady, receiver, [receiver, subscriber, value] {
                        receiver->deleteLater();
                        subscriber->setValue(value);
                        subscriber->update();
                    });
                }
            }); // QMetaObject and its consequences have been a disaster for the human race
    };

    updateWidget(nt::Event());

    NT_Listener listener = Globals::inst.AddListener(entry->GetTopic(), nt::EventFlags::kValueAll, updateWidget);

    topicListenerMap.insert({ntTopic, subscriber}, listener);

    return entry;
}

nt::NetworkTableEntry *TopicStore::subscribeWriteOnly(std::string ntTopic, BaseWidget *subscriber) {
    if (!topicEntryMap.contains(ntTopic)) {
        nt::NetworkTableEntry *entry =
            new nt::NetworkTableEntry(nt::GetEntry(Globals::inst.GetHandle(), ntTopic));

        topicEntryMap.insert(ntTopic, entry);
    }

    if (!topicSubscriberMap.contains(ntTopic, subscriber)) topicSubscriberMap.insert(ntTopic, subscriber);

    nt::NetworkTableEntry *entry = topicEntryMap.value(ntTopic);

    return entry;
}

void TopicStore::unsubscribe(std::string ntTopic, BaseWidget *subscriber) {
    if (!topicEntryMap.contains(ntTopic)) return;
    if (!topicSubscriberMap.contains(ntTopic, subscriber)) return;

    topicSubscriberMap.remove(ntTopic, subscriber);

    std::pair listenerMapPair = {ntTopic, subscriber};
    if (topicListenerMap.contains(listenerMapPair)) {
        Globals::inst.RemoveListener(topicListenerMap.value(listenerMapPair));
        topicListenerMap.remove(listenerMapPair);
    }

    if (!topicSubscriberMap.contains(ntTopic)) {
        nt::NetworkTableEntry *entry = topicEntryMap.value(ntTopic);

        entry->Unpublish();

        topicEntryMap.remove(ntTopic);
    }
}

void TopicStore::unsubscribe(nt::NetworkTableEntry *entry, BaseWidget *subscriber) {
    unsubscribe(entry->GetName(), subscriber);
}
