#ifndef FILTERSTORE_H
#define FILTERSTORE_H

#include <QObject>
#include <QMap>

#include "Globals.h"

class FilterStore : public QObject
{
    Q_OBJECT
private:
    static QMap<std::string, TopicTypes> m_sendableTypeMap;
    static QMap<nt::NetworkTableType, TopicTypes> m_ntTypeMap;

public:
    FilterStore();

    static void registerSendable(std::string typeString, TopicTypes topicType);
    static void registerNTType(nt::NetworkTableType ntType, TopicTypes topicType, const QString &displayName);

    static std::optional<TopicTypes> sendableTypeForTypeString(std::string typeString);
    static std::optional<TopicTypes> topicTypeForNTType(nt::NetworkTableType ntType);

    static QMap<QString, TopicTypes> FilteredTopics;

public slots:
    static void filterTopics();
    static void sortTopic(QString topic);

signals:

};

#endif // FILTERSTORE_H
