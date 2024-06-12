#ifndef TopicStore_H
#define TopicStore_H

#include "networktables/NetworkTableEntry.h"

#include <QMultiHash>
#include <QHash>
#include <QObject>

class BaseWidget;

class TopicStore : public QObject
{
    Q_OBJECT
public:
    static QHash<std::string, nt::NetworkTableEntry *> topicEntryMap;
    static QMultiHash<std::string, BaseWidget *> topicSubscriberMap;

    // little messy but its alright
    static QHash<std::pair<std::string, BaseWidget *>, NT_Listener> topicListenerMap;

    TopicStore();

    static nt::NetworkTableEntry *subscribe(std::string ntTopic, BaseWidget *subscriber);
    static nt::NetworkTableEntry *subscribeWriteOnly(std::string ntTopic, BaseWidget *subscriber);

    static void unsubscribe(std::string ntTopic, BaseWidget *subscriber);
    static void unsubscribe(nt::NetworkTableEntry *entry, BaseWidget *subscriber);
};

#endif // TopicStore_H
