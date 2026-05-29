// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Services/Logger.h"
#include "Services/TopicStore.h"
#include "SettingsManager.h"

#include <QGuiApplication>
#include <QSettings>

SettingsManager::SettingsManager(Logger* logs, TopicStore* store, QObject* parent)
    : QObject{parent}, m_logs(logs), m_store{store} {}

void SettingsManager::reconnect()
{
    std::string server = QString(Settings::IP).toStdString();
    int team = Settings::TeamNumber;
    int mode = Settings::ConnMode;

    switch (mode) {
    // IP Address
    case 0:
        m_store->setServer(server.c_str());
        m_logs->info("NT", "Requested connect to IP " + QString::fromStdString(server));
        break;
    // Team Number
    case 1:
        m_store->setServerTeam(team);
        m_logs->info("NT", "Requested connect to team number " + QString::number(team));
        break;
    // DS
    case 2:
        m_store->startDSClient();
        m_logs->info("NT", "Requested connect to DS");
        break;
    default:
        break;
    }

    m_store->disconnectServer();
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

QVariant Setting::value() const
{
    QSettings settings(qApp);
    return settings.value(Name, DefaultValue);
}

void Setting::setValue(const QVariant &value) const
{
    QSettings settings(qApp);
    settings.setValue(Name, value);
}

void Setting::operator=(const QVariant &value) const
{
    setValue(value);
}

Setting::operator QString() const
{
    return value().toString();
}

Setting::operator bool() const
{
    return value().toBool();
}

Setting::operator QStringList() const
{
    return value().toStringList();
}

Setting::operator double() const
{
    return value().toDouble();
}

Setting::operator int() const
{
    return value().toInt();
}

namespace Settings {
const Setting RecentFiles{"recentFiles", QStringList{}};
const Setting LoadRecent{"loadRecent", true};

const Setting Theme{"theme", 0};
const Setting Accent{"accent", 0};
const Setting Style{"style", "Graphide"};

const Setting WindowWidth{"windowWidth", 1000};
const Setting WindowHeight{"windowHeight", 640};
const Setting WindowX{"windowX", -1};
const Setting WindowY{"windowY", -1};

const Setting HannahMontanaMode{"hannahMontanaMode", false};

const Setting DefaultFontSize{"defaultFontSize", 20};
const Setting DefaultDisplayFontSize{"defaultDisplayFontSize", 100};
const Setting DefaultTitleFontSize{"defaultTitleFontSize", 16};

const Setting Scale{"scale", "1.0"};
const Setting ResizeToDS{"resizeToDS", "false"};
const Setting LogLevel{"logLevel", 2};
const Setting ConnMode{"connMode", 0};
const Setting TeamNumber{"teamNumber", "0"};
const Setting IP{"ip", "0.0.0.0"};
const Setting DisableWidgets{"disableWidgets", true};
}
