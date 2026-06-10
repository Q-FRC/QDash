// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QGuiApplication>

class QQmlApplicationEngine;
class CarboxylQuickInterface;
class NotificationHelper;
class PlatformHelper;
class ConnManager;
class TabListModel;
class SettingsManager;
class TopicStore;
class QSortFilterProxyModel;
class TopicListModel;
class CompileDefinitions;
class Logger;
class RemoteLayoutModel;

class QDashApplication : public QGuiApplication {
    Q_OBJECT
    Q_PROPERTY(bool shouldReload MEMBER m_shouldReload)
    Q_PROPERTY(QString dataLocation READ dataLocation CONSTANT FINAL)
public:
    QDashApplication(int &argc, char *argv[]);

    Q_INVOKABLE QString toLocalPath(const QString &path);

    QString dataLocation();

    Q_INVOKABLE QString wordToState(int val);

public slots:
    void reload();
    void gc();

    int run();

private:
    enum ControlWord {
        Invalid = 0x0,
        Enabled = 0x1,
        Auto = 0x2,
        Test = 0x4,
        EStop = 0x8,
        FMSAttached = 0x10,
        DSAttached = 0x20
    };

    QQmlApplicationEngine *m_engine;
    QWidget *m_widget;

    TopicListModel *topics;
    QSortFilterProxyModel *topicsSorted;

    TopicStore *store;
    Logger *logs;
    SettingsManager *settings;
    ConnManager *connManager;
    RemoteLayoutModel *remoteLayouts;

    CompileDefinitions *defs;

    TabListModel *tlm;
    PlatformHelper *platform;
    NotificationHelper *notification;

    bool m_shouldReload = false;
};
