#ifndef CONNMANAGER_H
#define CONNMANAGER_H

#include "BuildConfig.h"
#include <QObject>
#include <QQmlEngine>

class ConnManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ConnManager(QObject *parent = nullptr);

    QString title() const;
    QString status() const;

    bool connected() const;
    void setConnected(bool newConnected);

    QString address() const;
    void setAddress(const QString &newAddress);

private:
    QString m_status = "Not Connected";
    QString m_title = BuildConfig.APP_NAME + " - " + m_status;
    QString m_address = "0.0.0.0";
    bool m_connected;

    Q_PROPERTY(QString title READ title NOTIFY titleChanged FINAL)

    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged FINAL)

    Q_PROPERTY(QString status READ status NOTIFY statusChanged FINAL)

    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged FINAL)

signals:
    void titleChanged();
    void connectedChanged();
    void statusChanged();
    void addressChanged();
};

#endif // CONNMANAGER_H
