// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QFile>
#include <QObject>
#include <QVariant>

// TODO: Reorg this...
typedef struct Setting {
    Q_PROPERTY(QString name MEMBER Name CONSTANT FINAL)
    Q_PROPERTY(QVariant value READ value CONSTANT)
    Q_GADGET

public:
    const QString Name;
    const QVariant DefaultValue;

    QVariant value() const;
    void setValue(const QVariant &value) const;

    void operator=(const QVariant &value) const;

    operator QString() const;
    operator bool() const;
    operator QStringList() const;
    operator double() const;
    operator int() const;
} Setting;

namespace Settings
{
Q_NAMESPACE
extern const Setting RecentFiles;
extern const Setting LoadRecent;

extern const Setting Theme;
extern const Setting Accent;
extern const Setting Style;

extern const Setting WindowWidth;
extern const Setting WindowHeight;
extern const Setting WindowX;
extern const Setting WindowY;

extern const Setting HannahMontanaMode;

extern const Setting DefaultFontSize;
extern const Setting DefaultDisplayFontSize;
extern const Setting DefaultTitleFontSize;

extern const Setting Scale;
extern const Setting ResizeToDS;
extern const Setting LogLevel;
extern const Setting ConnMode;
extern const Setting TeamNumber;
extern const Setting IP;
extern const Setting DisableWidgets;
} // namespace Settings

class Logger;
class TopicStore;

// clang-format off
#define property(type, name, Name) Q_PROPERTY(type name READ name WRITE set##Name NOTIFY name##Changed FINAL) \
    Q_SIGNALS: void name##Changed(); \
    public Q_SLOTS: void set##Name(const type new##Name) {\
        Settings::Name = new##Name; \
        emit name##Changed(); \
    } \
    public: type name() const { \
        return Settings::Name; \
    }
// clang-format on

class SettingsManager : public QObject {
    Q_OBJECT

    Logger *m_logs;
    TopicStore *m_store;

    property(bool, loadRecent, LoadRecent);
    property(QStringList, recentFiles, RecentFiles);

    property(int, theme, Theme);
    property(int, accent, Accent);
    property(QString, style, Style);

    property(int, windowWidth, WindowWidth);
    property(int, windowHeight, WindowHeight);
    property(int, windowX, WindowX);
    property(int, windowY, WindowY);

    property(bool, hannahMontanaMode, HannahMontanaMode)

        property(int, defaultFontSize, DefaultFontSize);
    property(int, defaultDisplayFontSize, DefaultDisplayFontSize);
    property(int, defaultTitleFontSize, DefaultTitleFontSize);

    property(int, teamNumber, TeamNumber);
    property(int, connMode, ConnMode);
    property(QString, ip, IP);

    property(double, scale, Scale);
    property(bool, resizeToDS, ResizeToDS);
    property(int, logLevel, LogLevel);
    property(bool, disableWidgets, DisableWidgets);

public:
    explicit SettingsManager(Logger *logs, TopicStore *store, QObject *parent = nullptr);

    void addRecentFile(QFile &file);
    void addRecentFile(const QString &file);

    Q_INVOKABLE void reconnect();
};

#undef property
