#ifndef TopicStore_H
#define TopicStore_H

#include "logging/Logger.h"
#include "misc/Flags.h"

#include "networktables/NetworkTableEntry.h"

#include <QMultiHash>
#include <QHash>
#include <QObject>
#include <qqmlintegration.h>

struct Listener {
    QString topic;
    NT_Listener listenerHandle;
    nt::ListenerCallback callback;
    int numSubscribed;
    bool isNull;

    bool operator==(const Listener &other) const;
};

class TopicStore : public QObject
{
    Q_OBJECT
private:
    bool hasEntry(QString topic);

    Q_INVOKABLE Listener entry(QString topic);
    Q_INVOKABLE Listener changeNumSubscribed(QString topic, int changeBy = 1);

    QList<Listener> Listeners;
    QHash<QString, nt::NetworkTableEntry> topicEntryMap;

    Logger *m_logs;

public:
    static QVariant toVariant(const nt::Value &value);
    static nt::Value toValue(const QVariant &value);

    TopicStore(Logger *logs, QObject *parent = nullptr);

    void connect(bool connected);

    Q_INVOKABLE void subscribe(QString ntTopic);
    Q_INVOKABLE void unsubscribe(QString ntTopic);

    Q_INVOKABLE QVariant getValue(QString topic);
    Q_INVOKABLE void setValue(QString topic, const QVariant &value);

    Q_INVOKABLE void forceUpdate(const QString &topic);

    QString typeString(QString topic);

    Q_INVOKABLE inline QFDFlags::ControlWord toWord(int val)
    {
        return (QFDFlags::ControlWord) val;
    }
signals:
    void topicUpdate(QString topic, QVariant newValue);
    void connected(bool connected);
};

#endif // TopicStore_H
