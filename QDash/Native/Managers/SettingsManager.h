// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include "Logging/Logger.h"
#include "Misc/Constants.h"

#include <QFile>
#include <QObject>
#include <QQmlEngine>

#define property(type, name, Name) Q_PROPERTY(type name READ name WRITE set##Name NOTIFY name##Changed FINAL) \
    Q_SIGNALS: void name##Changed(); \
    public Q_SLOTS: void set##Name(const type new##Name) {\
        Settings::Name = new##Name; \
        emit name##Changed(); \
    } \
    public: type name() const { \
        return Settings::Name; \
    }

class SettingsManager : public QObject {
    Q_OBJECT

    Logger *m_logs;

    property(bool,        loadRecent,  LoadRecent);
    property(QStringList, recentFiles, RecentFiles);

    property(int,     theme,  Theme);
    property(int,     accent, Accent);
    property(QString, style,  Style);

    property(int, windowWidth,  WindowWidth);
    property(int, windowHeight, WindowHeight);
    property(int, windowX,      WindowX);
    property(int, windowY,      WindowY);

    property(int,     teamNumber, TeamNumber);
    property(int,     connMode,   ConnMode);
    property(QString, ip,         IP);

    property(double, scale,          Scale);
    property(bool,   resizeToDS,     ResizeToDS);
    property(int,    logLevel,       LogLevel);
    property(bool,   disableWidgets, DisableWidgets);

public:
    explicit SettingsManager(Logger *logs, QObject *parent = nullptr);

    void addRecentFile(QFile &file);
    void addRecentFile(const QString &file);

    Q_INVOKABLE void reconnect();
};

#undef property
#endif // SETTINGSMANAGER_H
