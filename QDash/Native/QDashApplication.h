// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QApplication>

class QQmlApplicationEngine;
class FileSelect;
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

class QDashApplication : public QApplication {
    Q_OBJECT
    Q_PROPERTY(bool shouldReload MEMBER m_shouldReload)
    Q_PROPERTY(QString dataLocation READ dataLocation CONSTANT FINAL)
public:
    QDashApplication(int &argc, char *argv[]);

    Q_INVOKABLE QString toLocalPath(const QString &path);

    QString dataLocation();

public slots:
    void reload();

    int run();

private:
    void setupNetworkTables();

    QQmlApplicationEngine *m_engine;
    QWidget *m_widget;

    TopicListModel *topics;
    QSortFilterProxyModel *topicsSorted;
    TopicStore *store;
    Logger *logs;
    SettingsManager *settings;
    CompileDefinitions *defs;

    TabListModel *tlm;
    ConnManager *conn;
    PlatformHelper *platform;
    NotificationHelper *notification;
    FileSelect *fileSelect;

    bool m_shouldReload = false;
};

