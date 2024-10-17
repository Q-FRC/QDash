#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <TabListModel.h>

#include "BuildConfig.h"
#include "CameraListModel.h"
#include "Flags.h"
#include "Globals.h"
#include "TitleManager.h"
#include "TopicListModel.h"
#include "TopicStore.h"

#include <QTimer>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setOrganizationName(BuildConfig.ORG_NAME);
    app.setApplicationName(BuildConfig.APP_NAME);
    app.setApplicationVersion(BuildConfig.versionString());

    TopicStore store(&app);
    TopicListModel *topics = new TopicListModel(store, &app);
    SettingsManager *settings = new SettingsManager(&app);
    TabListModel *tlm = new TabListModel(settings, &app);
    CameraListModel *clm = new CameraListModel(store, &app);
    TitleManager *title = new TitleManager(&app);

    Globals::inst.AddConnectionListener(true, [topics, &store, clm, title] (const nt::Event &event) {
        bool connected = event.Is(nt::EventFlags::kConnected);

        store.connect(connected);

        if (!connected) {
            QMetaObject::invokeMethod(topics, &TopicListModel::clear);
            QMetaObject::invokeMethod(clm, &CameraListModel::clear);
            title->resetTitle();
        } else {
            title->setTitle("Connected (" + QString::fromStdString(event.GetConnectionInfo()->remote_ip) + ")");
            QMetaObject::invokeMethod(clm, [=] {
                // kind of hacky, but what works, works
                QTimer::singleShot(1000, [=] {
                    clm->clear();

                    for (const std::string &st : Globals::inst.GetTable("/CameraPublisher")->GetSubTables()) {
                        std::shared_ptr<nt::NetworkTable> subtable = Globals::inst.GetTable("/CameraPublisher")->GetSubTable(st);

                        clm->add(subtable);
                    }
                });
            });
        }
    });

    Globals::inst.StartClient4(BuildConfig.APP_NAME.toStdString());
    Globals::inst.SetServer(Globals::server.server.c_str(), NT_DEFAULT_PORT4);
    Globals::inst.StartDSClient();

    Globals::inst.AddListener({{""}}, nt::EventFlags::kTopic, [topics] (const nt::Event &event) {
        std::string topicName(event.GetTopicInfo()->name);

        if (event.Is(nt::EventFlags::kPublish)) {
            QMetaObject::invokeMethod(topics, [topics, topicName] {
                topics->add(QString::fromStdString(topicName));
            });

        } else if (event.Is(nt::EventFlags::kUnpublish)) {
            // TODO: handle unpublishing
            // topics->remove(QString::fromStdString(topicName));
        }
    });

    qmlRegisterUncreatableMetaObject(
        QFDFlags::staticMetaObject,
        "QFDFlags",
        1, 0,
        "QFDFlags",
        "Attempt to create uninstantiable object \"QFDFlags\" ignored"
        );

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("topics", topics);
    engine.rootContext()->setContextProperty("settings", settings);
    engine.rootContext()->setContextProperty("cameras", clm);
    engine.rootContext()->setContextProperty("topicStore", &store);
    engine.rootContext()->setContextProperty("tlm", tlm);
    engine.rootContext()->setContextProperty("titleManager", title);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("QFRCDashboard", "Main");

    return app.exec();
}
