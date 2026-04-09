// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TopicStore_H
#define TopicStore_H

#include "Logging/Logger.h"
#include "Misc/Flags.h"

#include "networktables/NetworkTableEntry.h"

#include <QHash>
#include <QMultiHash>
#include <QObject>
#include <qjsvalue.h>
#include <qqmlintegration.h>

class Listener : public QObject {
    Q_OBJECT

public:
    Listener(QQmlEngine *engine, QString topic, QObject *parent);

    QString topic() const;

    /**
     * @brief addListener Add a subscriber to this listener.
     * @param func The function to call for this subscriber.
     */
    void addListener(const QJSValue& func);

    /**
     * @brief rmListener Remove a subscriber from this listener.
     * @param func The function to remove from the listener.
     * @return Whether or not the function was removed.
     */
    bool rmListener(const QJSValue& func);

    /**
     * @brief empty Check if this listener is empty.
     * @return Whether or not this listener has no associated functions.
     */
    bool empty();

    /**
     * @brief updateEvent Update this listener's subscribers from the provided event data.
     * @param event The NetworkTables event associated with this update.
     *   Set to a blank event or omit to automatically fetch the data from NT.
     */
    Q_INVOKABLE void updateEvent(const nt::Event& event = nt::Event());

    /**
     * @brief update Update all subscribers with the provided value.
     */
    void update(const QVariant& value);

    /**
     * @brief unpublish Unpublish this listener's associated NT entry,
     *  and remove the underlying handle.
     */
    void unpublish();

    /**
     * @brief setValue Set the value of this listener's associated NT entry.
     * @param value The value to publish to NetworkTables.
     */
    void setValue(const QVariant& value);

    /**
     * @brief getValue Get the value of this listener's associated NT entry.
     * @return The value retrieved from NetworkTables.
     */
    QVariant getValue();

    /**
     * @brief bindHandle Create the callback and bind the NT handle for this listener.
     */
    void bindHandle();

private:
    QString m_topic = {};
    NT_Listener m_handle = 0;
    nt::ListenerCallback m_callback = nt::ListenerCallback();
    QList<QJSValue> m_funcs = {};
    nt::NetworkTableEntry m_entry = nt::NetworkTableEntry{};
    QQmlEngine *m_engine = nullptr;

    bool operator==(const Listener& other) const;
};

class TopicStore : public QObject {
    Q_OBJECT
private:
    Q_INVOKABLE Listener* entry(QString topic);

    QList<Listener *> Listeners;

    Logger* m_logs;
    QQmlEngine *m_engine;

public:
    static QVariant toVariant(const nt::Value& value);
    static nt::Value toValue(const QVariant& value);

    TopicStore(QQmlEngine *engine, Logger* logs, QObject* parent = nullptr);

    void connect(bool connected);

    // The QJSValue is the function this subscription is connected to
    // So each subscription's unique ID is just the function itself.
    Q_INVOKABLE void subscribe(QString ntTopic, const QJSValue& func);
    Q_INVOKABLE void unsubscribe(QString ntTopic, const QJSValue& func);

    Q_INVOKABLE void subscribeOneShot(QString ntTopic, std::function<void(QVariant)> callback);

    Q_INVOKABLE QVariant getValue(QString topic);
    Q_INVOKABLE void setValue(QString topic, const QVariant& value);

    Q_INVOKABLE void forceUpdate(const QString& topic);

    QString typeString(QString topic);

    Q_INVOKABLE inline QFDFlags::ControlWord toWord(int val) {
        return QFDFlags::ControlWord(val);
    }
signals:
    void connected(bool connected);
};

#endif // TopicStore_H
