// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ConnManager.h"

ConnManager::ConnManager(QObject *parent) : QObject{parent} {}

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

    const auto connectedStr = QStringLiteral("Connected (%1)").arg(m_address);
    const auto notConnectedStr = QStringLiteral("Not Connected");

    m_status = m_connected ? connectedStr : notConnectedStr;

    m_title = QStringLiteral("%1 - %2 %3")
                  .arg(BuildConfig.APPLICATION_NAME, m_status, m_modified ? "*" : "");

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

bool ConnManager::modified() const {
    return m_modified;
}

void ConnManager::setModified(bool newModified) {
    if (m_modified == newModified)
        return;
    m_modified = newModified;
    emit modifiedChanged(m_modified);

    m_title = QStringLiteral("%1 - %2 %3")
                  .arg(BuildConfig.APPLICATION_NAME, m_status, m_modified ? "*" : "");

    emit titleChanged();

}

QString ConnManager::status() const
{
    return m_status;
}
