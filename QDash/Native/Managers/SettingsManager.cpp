// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Misc/Constants.h"
#include "Misc/Globals.h"
#include "SettingsManager.h"

SettingsManager::SettingsManager(Logger *logs, QObject *parent) : QObject{parent}, m_logs(logs) {}

void SettingsManager::reconnect()
{
    std::string server = QString(Settings::IP).toStdString();
    int team = Settings::TeamNumber;
    int mode = Settings::ConnMode;

    switch (mode) {
    // IP Address
    case 0:
        Globals::inst.SetServer(server.c_str(), NT_DEFAULT_PORT4);
        m_logs->info("NT", "Requested connect to IP " + QString::fromStdString(server));
        break;
    // Team Number
    case 1:
        Globals::inst.SetServerTeam(team, NT_DEFAULT_PORT4);
        m_logs->info("NT", "Requested connect to team number " + QString::number(team));
        break;
    // DS
    case 2:
        Globals::inst.StartDSClient(NT_DEFAULT_PORT4);
        m_logs->info("NT", "Requested connect to DS");
        break;
    default:
        break;
    }

    Globals::inst.Disconnect();
}

void SettingsManager::addRecentFile(const QString& filename) {
    QStringList recentFiles = Settings::RecentFiles;
    int index = recentFiles.indexOf(filename);

    if (index != -1) {
        recentFiles.move(index, 0);
    } else {
        recentFiles.prepend(filename);
    }

    if (recentFiles.length() > 5) {
        recentFiles.removeLast();
    }

    Settings::RecentFiles = recentFiles;

    emit recentFilesChanged();
}

void SettingsManager::addRecentFile(QFile& file) {
    QString fileName = file.fileName();
    addRecentFile(fileName);
}
