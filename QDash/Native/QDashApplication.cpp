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
#include <QQmlApplicationEngine>
#include <QSortFilterProxyModel>
#include <QStandardPaths>
#include <QWidget>

#include "Helpers/CompileDefinitions.h"

static constexpr const int EXIT_RELOAD = -2;

QDashApplication::QDashApplication(int &argc, char *argv[]) : QApplication(argc, argv)
{
    QGuiApplication::setOrganizationName(BuildConfig.ORGANIZATION_NAME);
    QGuiApplication::setApplicationName(BuildConfig.APPLICATION_NAME);
    QGuiApplication::setApplicationVersion(BuildConfig.GIT_TAG);

    QGuiApplication::setWindowIcon(QIcon(":/" + BuildConfig.APPLICATION_NAME));
    QGuiApplication::setDesktopFileName("" + BuildConfig.APPLICATION_NAME);
}

QString QDashApplication::toLocalPath(const QString &path) {
    QString name = path;
#ifdef Q_OS_WINDOWS
    name.replace("file:///", "");
#else
    name.replace("file://", "");
#endif

    return name;
}

QString QDashApplication::dataLocation() {
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    dir.mkpath(".");
    return dir.absolutePath();
}

void QDashApplication::reload()
{
    exit(EXIT_RELOAD);
}

int QDashApplication::run()
{
    int ret = EXIT_SUCCESS;
    do {
        if (ret == EXIT_RELOAD)
            qmlClearTypeRegistrations();

        QObject *parent = new QObject(this);
        QWidget *widget = new QWidget;

        conn = new ConnManager(parent);
        platform = new PlatformHelper(parent);
        logs = new Logger(parent);
        defs = new CompileDefinitions(parent);
        fileSelect = new FileSelect(widget);

        store = new TopicStore(logs, parent);
        settings = new SettingsManager(logs, parent);
        notification = new NotificationHelper(logs, parent);

        topics = new TopicListModel(store, parent);
        topicsSorted = new QSortFilterProxyModel(parent);

        topicsSorted->setSourceModel(topics);
        topicsSorted->setFilterRole(TopicListModel::TLMRoleTypes::TOPIC);
        topicsSorted->setFilterCaseSensitivity(Qt::CaseInsensitive);
        topicsSorted->setRecursiveFilteringEnabled(true);

        tlm = new TabListModel(logs, settings, parent);

        /// NETWORKTABLES //
        setupNetworkTables();

        QQmlApplicationEngine engine;

        // carboxyl setup
        CarboxylApplication *carboxylApp =
            new CarboxylApplication(*this, &engine, settings->style(), QStringLiteral("Graphide"));
        carboxylApp->setParent(this);

        /// CONTEXT
        auto ctx = engine.rootContext();

        engine.rootContext()->setContextProperty("topics", topics);
        engine.rootContext()->setContextProperty("topicsSorted", topicsSorted);
        engine.rootContext()->setContextProperty("QDashSettings", settings);
        engine.rootContext()->setContextProperty("TopicStore", store);
        engine.rootContext()->setContextProperty("tlm", tlm);
        engine.rootContext()->setContextProperty("conn", conn);
        engine.rootContext()->setContextProperty("platformHelper", platform);
        engine.rootContext()->setContextProperty("NotificationHelper", notification);
        engine.rootContext()->setContextProperty("buildConfig", &BuildConfig);
        engine.rootContext()->setContextProperty("logs", logs);
        engine.rootContext()->setContextProperty("FileSelect", fileSelect);

        // Enums
        qmlRegisterUncreatableMetaObject(
            QFDFlags::staticMetaObject, "QFDFlags", 1, 0, "QFDFlags",
            "Attempt to create uninstantiable object \"QFDFlags\" ignored");

        // :)
        ctx->setContextProperty(QStringLiteral("QDashApplication"), this);

        /// LOAD
        QObject::connect(
            &engine, &QQmlApplicationEngine::objectCreationFailed, this,
            []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

        engine.loadFromModule("QDash.Main", "Main");

        ret = exec();

    } while (ret == EXIT_RELOAD);

    return ret;
}

void QDashApplication::setupNetworkTables() {
    // Globals::inst = nt::NetworkTableInstance::Create();
    Globals::inst.StartClient4(BuildConfig.APPLICATION_NAME.toStdString());
    Globals::inst.StartDSClient(NT_DEFAULT_PORT4);

    Globals::inst.AddConnectionListener(true, [this](const nt::Event &event) {
        logs->info("NT", "PLOOOOOO");
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

    Globals::inst.AddListener({{""}}, nt::EventFlags::kTopic, [this](const nt::Event &event) {
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
    Globals::inst.AddListener(tabEntry, nt::EventFlags::kValueAll, [this](const nt::Event &event) {
        std::string_view value = event.GetValueEventData()->value.GetString();
        QString qvalue = QString::fromStdString(std::string{value});

        QMetaObject::invokeMethod(tlm, [this, qvalue] {
            tlm->selectTab(qvalue);
            logs->debug("NT", "Requested tab switch to tab " + qvalue);
        });
    });

    nt::NetworkTableEntry notificationEntry = Globals::inst.GetEntry("/QDash/RobotNotifications");

    Globals::inst.AddListener(
        notificationEntry, nt::EventFlags::kValueAll, [this](const nt::Event &event) {
            std::string_view value = event.GetValueEventData()->value.GetString();
            QString qvalue = QString::fromStdString(std::string{value});
            QJsonDocument doc = QJsonDocument::fromJson(qvalue.toUtf8());

            QMetaObject::invokeMethod(notification, [doc, this, qvalue] {
                notification->fromJson(doc);
                logs->debug("Notifications", "Received notification data " + qvalue);
            });
        });
}
