#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSortFilterProxyModel>

#include "models/TabListModel.h"
#include "models/AccentsListModel.h"
#include "models/TopicListModel.h"

#include "buildconfig/BuildConfig.h"

#include "managers/ConnManager.h"

#include "logging/Logger.h"

#include "misc/Flags.h"
#include "misc/Globals.h"

#include "helpers/NotificationHelper.h"
#include "helpers/PlatformHelper.h"

#include "nt/TopicStore.h"

#include <QTimer>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setOrganizationName(BuildConfig.ORGANIZATION_NAME);
    app.setApplicationName(BuildConfig.APPLICATION_NAME);
    app.setApplicationVersion(BuildConfig.GIT_TAG);

    app.setWindowIcon(QIcon(":/" + BuildConfig.APPLICATION_NAME));
    app.setDesktopFileName("" + BuildConfig.APPLICATION_NAME);

    QQuickStyle::setStyle("Material");

    // TODO: Holy nuts please organize this
    QObject *parent = new QObject(&app);

    Logger *logs = new Logger(parent);

    TopicStore store(logs, parent);

    TopicListModel *topics = new TopicListModel(store, parent);
    QSortFilterProxyModel *topicsSorted = new QSortFilterProxyModel(parent);

    topicsSorted->setSourceModel(topics);
    topicsSorted->setFilterRole(TopicListModel::TLMRoleTypes::TOPIC);
    topicsSorted->setFilterCaseSensitivity(Qt::CaseInsensitive);
    topicsSorted->setRecursiveFilteringEnabled(true);

    SettingsManager *settings = new SettingsManager(logs, parent);

    TabListModel *tlm = new TabListModel(logs, settings, parent);

    ConnManager *conn = new ConnManager(parent);

    AccentsListModel *accents = new AccentsListModel(parent);
    accents->load();

    PlatformHelper *platform = new PlatformHelper(parent);

    NotificationHelper *notification = new NotificationHelper(parent);

    Globals::inst.AddConnectionListener(true, [topics, &store, conn, logs](const nt::Event &event) {
        bool connected = event.Is(nt::EventFlags::kConnected);

        store.connect(connected);

        auto connInfo = event.GetConnectionInfo();
        QString remoteIP = QString::fromStdString(connInfo->remote_ip);

        QMetaObject::invokeMethod(topics, [remoteIP, connected, logs, topics, conn] {
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

    Globals::inst.StartClient4(BuildConfig.APPLICATION_NAME.toStdString());
    Globals::inst.StartDSClient(NT_DEFAULT_PORT4);

    Globals::inst.AddListener({{""}}, nt::EventFlags::kTopic, [topics, logs](const nt::Event &event) {
        std::string topicName(event.GetTopicInfo()->name);

        if (event.Is(nt::EventFlags::kPublish)) {
            QMetaObject::invokeMethod(topics, [topics, topicName, logs] {
                logs->debug("NT", "Received topic announcement for " + QString::fromStdString(topicName));
                topics->add(QString::fromStdString(topicName));
            });

        } else if (event.Is(nt::EventFlags::kUnpublish)) {
            // TODO: handle unpublishing
            // topics->remove(QString::fromStdString(topicName));
        }
    });

    nt::NetworkTableEntry tabEntry = Globals::inst.GetEntry("/QDash/Tab");
    Globals::inst.AddListener(tabEntry, nt::EventFlags::kValueAll, [tlm, logs](const nt::Event &event) {
        std::string_view value = event.GetValueEventData()->value.GetString();
        QString qvalue = QString::fromStdString(std::string{value});


        QMetaObject::invokeMethod(tlm, [tlm, qvalue, logs] {
            tlm->selectTab(qvalue);
            logs->debug("NT", "Requested tab switch to tab " + qvalue);
        });
    });

    nt::NetworkTableEntry notificationEntry = Globals::inst.GetEntry(
        "/QDash/RobotNotifications");

    Globals::inst.AddListener(notificationEntry,
                              nt::EventFlags::kValueAll,
                              [tlm, parent, notification, logs](const nt::Event &event) {
                                  std::string_view value = event.GetValueEventData()
                                  ->value.GetString();
                                  QString qvalue = QString::fromStdString(std::string{value});
                                  QJsonDocument doc = QJsonDocument::fromJson(qvalue.toUtf8());

                                  QMetaObject::invokeMethod(notification, [doc, notification, logs, qvalue] {
                                      notification->fromJson(doc);
                                      logs->debug("Notifications", "Received notification data " + qvalue);
                                  });
                              });

    qmlRegisterUncreatableMetaObject(
        QFDFlags::staticMetaObject,
        "QFDFlags",
        1,
        0,
        "QFDFlags",
        "Attempt to create uninstantiable object \"QFDFlags\" ignored");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("topics", topics);
    engine.rootContext()->setContextProperty("topicsSorted", topicsSorted);
    engine.rootContext()->setContextProperty("settings", settings);
    engine.rootContext()->setContextProperty("topicStore", &store);
    engine.rootContext()->setContextProperty("tlm", tlm);
    engine.rootContext()->setContextProperty("conn", conn);
    engine.rootContext()->setContextProperty("accents", accents);
    engine.rootContext()->setContextProperty("platformHelper", platform);
    engine.rootContext()->setContextProperty("notificationHelper", notification);
    engine.rootContext()->setContextProperty("buildConfig", &BuildConfig);
    engine.rootContext()->setContextProperty("logs", logs);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        parent,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("main", "Main");

    logs->info("QDash", "Application started");

    return app.exec();
}
