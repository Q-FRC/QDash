#include "ConnManager.h"

ConnManager::ConnManager(QObject *parent)
    : QObject{parent}
{}

QString ConnManager::title() const
{
    return m_title;
}

bool ConnManager::connected() const
{
    return m_connected;
}

void ConnManager::setConnected(bool newConnected)
{
    if (m_connected == newConnected)
        return;
    m_connected = newConnected;

    m_status = QString(newConnected ? "" : "Not ") + "Connected" + (newConnected ? " (" + m_address + ")" : "");
    m_title = BuildConfig.APP_NAME + " - " + m_status;

    emit titleChanged();
    emit statusChanged();
    emit connectedChanged();
}

QString ConnManager::address() const
{
    return m_address;
}

void ConnManager::setAddress(const QString &newAddress)
{
    if (m_address == newAddress)
        return;
    m_address = newAddress;
    emit addressChanged();
}

QString ConnManager::status() const
{
    return m_status;
}

