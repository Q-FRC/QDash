// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "BuildConfig.h"
#include "Helpers/FileSelect.h"
#include "Helpers/NotificationHelper.h"
#include "Helpers/PlatformHelper.h"
#include "Logging/Logger.h"
#include "Managers/ConnManager.h"
#include "Managers/SettingsManager.h"
#include "Misc/Globals.h"
#include "Models/TabListModel.h"
#include "Models/TopicListModel.h"
#include "NT/TopicStore.h"
#include "QDashApplication.h"

#include <CarboxylApplication.h>
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

    conn = new ConnManager(this);
    platform = new PlatformHelper(this);
    logs = new Logger(this);
    defs = new CompileDefinitions(this);
    fileSelect = new FileSelect(m_widget);

    store = new TopicStore(m_engine, logs, this);
    settings = new SettingsManager(logs, this);
    notification = new NotificationHelper(logs, this);

    topics = new TopicListModel(store, this);
    topicsSorted = new QSortFilterProxyModel(this);

    topicsSorted->setSourceModel(topics);
    topicsSorted->setFilterRole(TopicListModel::TLMRoleTypes::TOPIC);
    topicsSorted->setFilterCaseSensitivity(Qt::CaseInsensitive);
    topicsSorted->setRecursiveFilteringEnabled(true);

    tlm = new TabListModel(logs, settings, this);

    /// NETWORKTABLES //
    setupNetworkTables();

    // carboxyl setup
    CarboxylApplication* carboxylApp =
        new CarboxylApplication(*this, m_engine, settings->style(), QStringLiteral("Graphide"));
    carboxylApp->setParent(this);

    /// CONTEXT
    auto ctx = m_engine->rootContext();

    ctx->setContextProperty("topics", topics);
    ctx->setContextProperty("topicsSorted", topicsSorted);
    ctx->setContextProperty("QDashSettings", settings);
    ctx->setContextProperty("TopicStore", store);
    ctx->setContextProperty("tlm", tlm);
    ctx->setContextProperty("conn", conn);
    ctx->setContextProperty("platformHelper", platform);
    ctx->setContextProperty("NotificationHelper", notification);
    ctx->setContextProperty("buildConfig", &BuildConfig);
    ctx->setContextProperty("logs", logs);
    ctx->setContextProperty("FileSelect", fileSelect);

    // Enums
    qmlRegisterUncreatableMetaObject(
        QFDFlags::staticMetaObject, "QFDFlags", 1, 0, "QFDFlags",
        "Attempt to create uninstantiable object \"QFDFlags\" ignored");

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

void QDashApplication::reload() {
    qDebug() << "Reload called";
    QString program = QApplication::applicationFilePath();
    QProcess::startDetached(program, QApplication::arguments().mid(1));
    exit(0);
}

void QDashApplication::setupNetworkTables() {
    // Globals::inst = nt::NetworkTableInstance::Create();
    Globals::inst.StartClient4(BuildConfig.APPLICATION_NAME.toStdString());
    Globals::inst.StartDSClient(NT_DEFAULT_PORT4);

    Globals::inst.AddConnectionListener(true, [this](const nt::Event& event) {
        bool connected = event.Is(nt::EventFlags::kConnected);

        store->connect(connected);

        auto connInfo = event.GetConnectionInfo();
        QString remoteIP = QString::fromStdString(connInfo->remote_ip);

        QMetaObject::invokeMethod(topics, [remoteIP, connected, this] {
            logs->info("NT", QString("Connected State: ") + (connected ? "true" : "false"));

            if (!connected) {
                topics->clear();
            } else {
                conn->setAddress(remoteIP);
                logs->info("NT", "Client connected to " + remoteIP);
            }
            conn->setConnected(connected);
        });
    });

    Globals::inst.AddListener({{""}}, nt::EventFlags::kTopic, [this](const nt::Event& event) {
        std::string topicName(event.GetTopicInfo()->name);

        if (event.Is(nt::EventFlags::kPublish)) {
            QMetaObject::invokeMethod(topics, [this, topicName] {
                logs->debug("NT",
                            "Received topic announcement for " + QString::fromStdString(topicName));
                topics->add(QString::fromStdString(topicName));
            });

        } else if (event.Is(nt::EventFlags::kUnpublish)) {
            // TODO: handle unpublishing
            // topics->remove(QString::fromStdString(topicName));
        }
    });

    nt::NetworkTableEntry tabEntry = Globals::inst.GetEntry("/QDash/Tab");
    Globals::inst.AddListener(tabEntry, nt::EventFlags::kValueAll, [this](const nt::Event& event) {
        std::string_view value = event.GetValueEventData()->value.GetString();
        QString qvalue = QString::fromStdString(std::string{value});

        QMetaObject::invokeMethod(tlm, [this, qvalue] {
            tlm->selectTab(qvalue);
            logs->debug("NT", "Requested tab switch to tab " + qvalue);
        });
    });

    nt::NetworkTableEntry notificationEntry = Globals::inst.GetEntry("/QDash/RobotNotifications");

    Globals::inst.AddListener(
        notificationEntry, nt::EventFlags::kValueAll, [this](const nt::Event& event) {
            std::string_view value = event.GetValueEventData()->value.GetString();
            QString qvalue = QString::fromStdString(std::string{value});
            QJsonDocument doc = QJsonDocument::fromJson(qvalue.toUtf8());

            QMetaObject::invokeMethod(notification, [doc, this, qvalue] {
                notification->fromJson(doc);
                logs->debug("Notifications", "Received notification data " + qvalue);
            });
        });
}
