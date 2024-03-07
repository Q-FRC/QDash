#include <QApplication>
#include <QTimer>
#include <QRadioButton>
#include <QMessageBox>
#include <QCommandLineParser>
#include <QStyleFactory>

#include "widgets/BooleanCheckboxWidget.h"
#include "widgets/BooleanDisplayWidget.h"
#include "widgets/CameraViewWidget.h"
#include "widgets/DoubleDialWidget.h"
#include "widgets/EnumWidget.h"
#include "widgets/DoubleDisplayWidget.h"
#include "widgets/StringChooserWidget.h"
#include "widgets/StringDisplayWidget.h"
#include "widgets/IntegerDisplayWidget.h"
#include "widgets/IntegerDialWidget.h"
#include "widgets/FieldWidget.h"
#include "widgets/SendableFieldWidget.h"
#include "widgets/CommandWidget.h"
#include "widgets/GraphWidget.h"
#include "widgets/FMSInfoWidget.h"
#include "widgets/SwerveWidget.h"

#include "MainWindow.h"
#include "Globals.h"
#include "stores/FilterStore.h"
#include "stores/TypeStore.h"

#include "Constants.h"

#include <SingleApplication>
#include <BuildConfig.h>

int main(int argc, char **argv) {
    SingleApplication app(argc, argv);

    Q_INIT_RESOURCE(breeze);

    app.setOrganizationName(BuildConfig.ORG_NAME);
    app.setApplicationName(BuildConfig.APP_NAME);
    app.setApplicationVersion(BuildConfig.versionString());

    QCommandLineParser parser;
    parser.setApplicationDescription("Simple, reliable, high-performance, low-footprint FRC dashboard");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "JSON file to open");

    parser.process(app);
    // i hate macos
#ifdef Q_OS_MACOS
    app.setStyle(QStyleFactory::create("Fusion"));
#endif

    MainWindow *window = new MainWindow();
    window->show();

    Globals::inst.AddConnectionListener(true, [window] (const nt::Event &event) {
        bool connected = event.Is(nt::EventFlags::kConnected);

        QMetaObject::invokeMethod(window, [window, connected] {
            window->setWindowTitle(
                QString("%1 %2 (%3) - %4")
                    .arg(BuildConfig.APP_NAME,
                         BuildConfig.versionString(),
                         QString::fromStdString(Globals::server.server),
                         QString(connected ? "" : "Not ") + "Connected")
                );

            if (connected) window->reconnect();
        });
    });

    Globals::inst.StartClient4(BuildConfig.APP_NAME.toStdString());
    Globals::inst.SetServer(Globals::server.server.c_str(), NT_DEFAULT_PORT4);
    Globals::inst.StartDSClient();

    // NT REGISTRATION

    FilterStore::registerNTType(nt::NetworkTableType::kBoolean, TopicTypes::Boolean, "Boolean");
    FilterStore::registerNTType(nt::NetworkTableType::kString, TopicTypes::String, "String");
    FilterStore::registerNTType(nt::NetworkTableType::kDouble, TopicTypes::Double, "Double");
    FilterStore::registerNTType(nt::NetworkTableType::kDoubleArray, TopicTypes::DoubleArray, "Double Array");
    FilterStore::registerNTType(nt::NetworkTableType::kStringArray, TopicTypes::StringArray, "String Array");
    FilterStore::registerNTType(nt::NetworkTableType::kInteger, TopicTypes::Int, "Integer");

    // NT REGISTRATION

    // Funky
    auto register_widget_types = []<typename... Widget>() {
        auto register_widget = [&]<typename WidgetType>() -> bool {
            auto topic = WidgetType::TopicType;
            auto widget = WidgetType::WidgetType;
            auto sendable = WidgetType::SendableName;
            auto name = WidgetType::DisplayName;

            if (widget != WidgetTypes::None && topic != TopicTypes::None) {
                Globals::typeStore.registerType(topic, widget, name);
            }

            if (!sendable.isEmpty()) {
                FilterStore::registerSendable(sendable.toStdString(), topic);
            }

            return false;
        };
        (register_widget.template operator()<Widget>() || ...);
    };

    register_widget_types.template operator()<BooleanCheckboxWidget,
                                              BooleanDisplayWidget,
                                              DoubleDisplayWidget,
                                              StringDisplayWidget,
                                              DoubleDialWidget,
                                              StringChooserWidget,
                                              CameraViewWidget,
                                              EnumWidget,
                                              IntegerDisplayWidget,
                                              IntegerDialWidget,
                                              FieldWidget,
                                              SendableFieldWidget,
                                              CommandWidget,
                                              GraphWidget,
                                              FMSInfoWidget,
                                              SwerveWidget>();

    // settings dont exist or firstrun is true
    bool firstRun = Settings::FirstRun.value().toBool();

    if (firstRun) {
        Settings::FirstRun.setValue(false);

        QMessageBox::information
            (window, QString("Welcome to %1!").arg(BuildConfig.APP_NAME),
             QString("Welcome to %1. Ensure to check the GitHub page in the \"about\" "
                     "tab.\n\n"
                     "To get started, open the NT server settings tab with Alt+S and input your desired "
                     "NetworkTables settings. Once connected, add a tab with Ctrl+T, and search "
                     "for your widget in the New Widget tab with Alt+N.\n\n"

                     "Note that row and column indices start AT 0! Spans, however, do not.\n\n"

                     "Save and load at any time with Ctrl+S and Ctrl+O, or through the File Menu. "
                     "Close tabs with Ctrl+W or through the Tab menu. Modify or delete widgets--"
                     "or even change their fonts--by right clicking any widget.")
                 .arg(BuildConfig.APP_NAME));
    }

    QString styleSheet = Settings::StyleSheet.value().toString();

    setAppStyleSheet(styleSheet);

    window->refreshRecentFiles();

    Globals::inst.AddListener({{""}}, nt::EventFlags::kTopic, [window] (const nt::Event &event) {
        std::string topicName(event.GetTopicInfo()->name);

        if (event.Is(nt::EventFlags::kPublish)) {
            Globals::ntTopics.append(QString::fromStdString(topicName));
            FilterStore::filterTopics();
        } else if (event.Is(nt::EventFlags::kUnpublish)) {
            Globals::ntTopics.removeOne(QString::fromStdString(topicName));
            FilterStore::filterTopics();
        }
    });

    QObject::connect(
        &app,
        &SingleApplication::instanceStarted,
        window,
        &QMainWindow::raise
        );

    const QStringList args = parser.positionalArguments();
    if (!args.isEmpty()) {
        QFile file(args.at(0));
        window->open(file);
    } else {
        bool loadRecent = Settings::LoadRecent.value().toBool();
        if (loadRecent) {
            QStringList recent = Settings::RecentFiles.value().toStringList();
            if (!recent.empty()) {
                QFile file(recent.first());
                window->open(file);
            }
        }
    }

    auto makeListener = [window]() -> auto {
        std::string topic = Globals::server.switchTopic.toStdString();

        return Globals::inst.AddListener(Globals::inst.GetEntry(topic), nt::EventFlags::kValueAll, [window](const nt::Event &event) {
            std::string value = std::string{event.GetValueEventData()->value.GetString()};
            TabWidget *tab = window->tabNamed(QString::fromStdString(value));
            if (tab != nullptr) {
                window->selectTab(tab);
            }
        });
    };

    NT_Listener listener = makeListener();

    QObject::connect(window, &MainWindow::switchTopicChanged, &app, [listener, makeListener]() mutable {
        Globals::inst.RemoveListener(listener);
        listener = makeListener();
    });

    return app.exec();
}
