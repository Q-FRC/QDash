#include <QApplication>
#include <QTimer>
#include <QRadioButton>
#include <QSettings>
#include <QMessageBox>
#include <QCommandLineParser>

#include "MainWindow.h"
#include "Globals.h"
#include "stores/FilterStore.h"
#include "stores/TypeStore.h"

#include <SingleApplication>
#include <BuildConfig.h>

int main(int argc, char **argv) {
    SingleApplication app(argc, argv);

    app.setOrganizationName(BuildConfig.ORG_NAME);
    app.setApplicationName(BuildConfig.APP_NAME);
    app.setApplicationVersion(BuildConfig.versionString());

    QCommandLineParser parser;
    parser.setApplicationDescription("Simple, reliable, high-performance, low-footprint FRC dashboard");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "JSON file to open");

    parser.process(app);

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
        });
    });

    Globals::inst.StartClient4(BuildConfig.APP_NAME.toStdString());
    Globals::inst.SetServer(Globals::server.server.c_str(), NT_DEFAULT_PORT4);

// NT REGISTRATION
#define REGISTER_NT(ntType, topicType, displayName) FilterStore::registerNTType(ntType, topicType, displayName);

    REGISTER_NT(nt::NetworkTableType::kBoolean, TopicTypes::Boolean, "Boolean")
    REGISTER_NT(nt::NetworkTableType::kString, TopicTypes::String, "String")
    REGISTER_NT(nt::NetworkTableType::kDouble, TopicTypes::Double, "Double")
    REGISTER_NT(nt::NetworkTableType::kDoubleArray, TopicTypes::DoubleArray, "Double Array")
    REGISTER_NT(nt::NetworkTableType::kInteger, TopicTypes::Int, "Integer")

#undef REGISTER_NT

// SENDABLE REGISTRATION
#define REGISTER_SENDABLE(typeString, topicType) FilterStore::registerSendable(typeString, topicType);

    REGISTER_SENDABLE("String Chooser", TopicTypes::SendableChooser)
    REGISTER_SENDABLE("Field2d", TopicTypes::Field2d)
    REGISTER_SENDABLE("Command", TopicTypes::Command)


#undef REGISTER_SENDABLE

// WIDGET REGISTRATION
#define REGISTER_TYPE(topic, widget, name) Globals::typeStore.registerType(topic, widget, name);

    REGISTER_TYPE(TopicTypes::Boolean, WidgetTypes::BooleanCheckbox, "Checkbox")
    REGISTER_TYPE(TopicTypes::Boolean, WidgetTypes::BooleanDisplay, "Color Display")

    REGISTER_TYPE(TopicTypes::Double, WidgetTypes::DoubleDial, "Dial")
    REGISTER_TYPE(TopicTypes::Double, WidgetTypes::DoubleDisplay, "Double Display")

    REGISTER_TYPE(TopicTypes::DoubleArray, WidgetTypes::Field, "Field2d")

    REGISTER_TYPE(TopicTypes::String, WidgetTypes::StringDisplay, "Text Display")
    REGISTER_TYPE(TopicTypes::String, WidgetTypes::EnumWidget, "Enum")

    REGISTER_TYPE(TopicTypes::Int, WidgetTypes::IntegerDisplay, "Integer Display")
    REGISTER_TYPE(TopicTypes::Int, WidgetTypes::IntegerDial, "Dial")

    REGISTER_TYPE(TopicTypes::SendableChooser, WidgetTypes::SendableChooser, "Sendable Chooser");

    REGISTER_TYPE(TopicTypes::Field2d, WidgetTypes::SendableField, "Field2d");

    REGISTER_TYPE(TopicTypes::Command, WidgetTypes::Command, "Command");

#undef REGISTER_TYPE

    QSettings settings(&app);

    // settings dont exist or firstrun is true
    bool firstRun = !settings.contains("firstRun") || settings.value("firstRun").toBool();

    if (firstRun) {
        settings.setValue("firstRun", false);

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

    Globals::inst.AddListener({{""}}, nt::EventFlags::kTopic, [window] (const nt::Event &event) {
        std::string topicName(event.GetTopicInfo()->name);

        if (event.Is(nt::EventFlags::kPublish)) {
            Globals::ntTopics.append(QString::fromStdString(topicName));
        } else if (event.Is(nt::EventFlags::kUnpublish)) {
            Globals::ntTopics.removeOne(QString::fromStdString(topicName));
        }

        FilterStore::filterTopics();
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
    }

    return app.exec();
}
