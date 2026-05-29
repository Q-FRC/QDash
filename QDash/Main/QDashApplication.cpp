// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "BuildConfig.h"
#include "Helpers/FileSelect.h"
#include "Helpers/NotificationHelper.h"
#include "Helpers/PlatformHelper.h"
#include "Models/RemoteLayoutModel.h"
#include "Services/Logger.h"
#include "Services/ConnManager.h"
#include "Services/SettingsManager.h"
#include "Models/TabListModel.h"
#include "Models/TopicListModel.h"
#include "Services/TopicStore.h"
#include "QDashApplication.h"

#include <CarboxylApplication.h>
#include <CarboxylQuickInterface.h>
#include <QIcon>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSortFilterProxyModel>
#include <QStandardPaths>
#include <QWidget>
#include "Helpers/CompileDefinitions.h"

static constexpr const int EXIT_RELOAD = -2;

QDashApplication::QDashApplication(int& argc, char* argv[]) : QApplication(argc, argv) {
    QGuiApplication::setOrganizationName(BuildConfig.ORGANIZATION_NAME);
    QGuiApplication::setApplicationName(BuildConfig.APPLICATION_NAME);
    QGuiApplication::setApplicationVersion(BuildConfig.GIT_TAG);

    QGuiApplication::setWindowIcon(QIcon(":/" + BuildConfig.APPLICATION_NAME));
    QGuiApplication::setDesktopFileName("" + BuildConfig.APPLICATION_NAME);

    m_engine = new QQmlApplicationEngine(this);
    m_widget = new QWidget;

    platform = new PlatformHelper(this);
    logs = new Logger(this);
    defs = new CompileDefinitions(this);
    fileSelect = new FileSelect(m_widget);

    // TODO: Move logging to singletons, and expose simple log functions to QML

    store = new TopicStore(m_engine, logs, this);
    connManager = new ConnManager(store, this);
    settings = new SettingsManager(logs, store, this);
    notification = new NotificationHelper(logs, this);
    remoteLayouts = new RemoteLayoutModel(store, this);

    topics = new TopicListModel(store, this);
    topicsSorted = new QSortFilterProxyModel(this);

    topicsSorted->setSourceModel(topics);
    topicsSorted->setFilterRole(TopicListModel::TLMRoleTypes::TOPIC);
    topicsSorted->setFilterCaseSensitivity(Qt::CaseInsensitive);
    topicsSorted->setRecursiveFilteringEnabled(true);

    tlm = new TabListModel(logs, settings, this);

    // carboxyl setup
    CarboxylApplication* carboxylApp =
        new CarboxylApplication(*this, m_engine, settings->style(), QStringLiteral("Graphide"), true);
    carboxylApp->setParent(this);

    /// CONTEXT
    auto ctx = m_engine->rootContext();

    ctx->setContextProperty("topics", topics);
    ctx->setContextProperty("topicsSorted", topicsSorted);
    ctx->setContextProperty("QDashSettings", settings);
    ctx->setContextProperty("TopicStore", store);
    ctx->setContextProperty("TabListModel", tlm);
    ctx->setContextProperty("ConnManager", connManager);
    ctx->setContextProperty("PlatformHelper", platform);
    ctx->setContextProperty("RemoteLayoutModel", remoteLayouts);
    ctx->setContextProperty("NotificationHelper", notification);
    ctx->setContextProperty("CompileDefinitions", defs);
    ctx->setContextProperty("buildConfig", &BuildConfig);
    ctx->setContextProperty("logs", logs);
    ctx->setContextProperty("FileSelect", fileSelect);

    // :)
    ctx->setContextProperty(QStringLiteral("QDashApplication"), this);

    /// LOAD
    QObject::connect(
        m_engine, &QQmlApplicationEngine::objectCreationFailed, this,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
}

QString QDashApplication::toLocalPath(const QString& path) {
    QString name = path;
#ifdef Q_OS_WINDOWS
    name.replace("file:///", "");
#else
    name.replace("file://", "");
#endif

    return name;
}

int QDashApplication::run() {
    m_engine->loadFromModule("QDash.Main", "Main");
    int ret = exec();
    m_widget->deleteLater();
    return ret;
}

QString QDashApplication::dataLocation() {
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    dir.mkpath(".");
    return dir.absolutePath();
}

QString QDashApplication::wordToState(int val) {
    auto word = ControlWord(val);

    QString mode, state;

    if (word & Auto) {
        mode = "Autonomous";
    } else if (word & Test) {
        mode = "Testing";
    } else {
        mode = "Teleop";
    }

    if (word & Enabled) {
        state = "Enabled";
    } else if (word & EStop) {
        state = "E-Stopped";
    } else {
        state = "Disabled";
    }

    return QStringLiteral("%1 %2").arg(mode, state);
}

void QDashApplication::reload() {
    qDebug() << "Reload called";
    QString program = QApplication::applicationFilePath();
#if defined(TARGET_OS_IOS) || defined(__ANDROID__)
    qWarning() << "Platform does not support native reload, exiting instead";
#else
    QProcess::startDetached(program, QApplication::arguments().mid(1));
#endif
    exit(0);
}

// TODO(crueter): Bring this back, QML will handle this (maybe)
// nt::NetworkTableEntry notificationEntry = Globals::inst.GetEntry("/QDash/RobotNotifications");

// Globals::inst.AddListener(
//     notificationEntry, nt::EventFlags::kValueAll, [this](const nt::Event& event) {
//         std::string_view value = event.GetValueEventData()->value.GetString();
//         QString qvalue = QString::fromStdString(std::string{value});
//         QJsonDocument doc = QJsonDocument::fromJson(qvalue.toUtf8());

//         QMetaObject::invokeMethod(notification, [doc, this, qvalue] {
//             notification->fromJson(doc);
//             logs->debug("Notifications", "Received notification data " + qvalue);
//         });
//     });
