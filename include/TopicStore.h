#ifndef TopicStore_H
#define TopicStore_H

#include "networktables/NetworkTableEntry.h"

#include <QMultiHash>
#include <QHash>
#include <QObject>

class TopicStore : public QObject
{
    Q_OBJECT
private:
    static QHash<std::string, nt::NetworkTableEntry *> topicEntryMap;
    static QMultiHash<std::string, QObject *> topicSubscriberMap;
public:
    TopicStore();

    static nt::NetworkTableEntry *subscribe(std::string ntTopic, QObject *subscriber);

    static void unsubscribe(std::string ntTopic, QObject *subscriber);
    static void unsubscribe(nt::NetworkTableEntry *entry, QObject *subscriber);
};

#endif // TopicStore_H
