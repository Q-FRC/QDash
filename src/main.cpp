#include "ntcore_cpp.h"

#include <QApplication>
#include <QTimer>
#include <QRadioButton>
#include <QSettings>
#include <QMessageBox>

#include "MainWindow.h"
#include "Globals.h"
#include "stores/FilterStore.h"
#include "stores/TypeStore.h"

TypeStore *Globals::typeStore = new TypeStore;

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    app.setOrganizationName("binex-dsk");
    app.setApplicationName("QFRCDashboard");

    MainWindow *window = new MainWindow();
    window->show();

    Globals::inst.AddConnectionListener(true, [window] (const nt::Event &event) {
        bool connected = event.Is(nt::EventFlags::kConnected);

        QMetaObject::invokeMethod(window, [window, connected] {
            window->setWindowTitle("QFRCDashboard (" + QString::fromStdString(Globals::server.server) + ") - " + (connected ? "" : "Not ") + "Connected");
        });
    });

    Globals::inst.StartClient4("QFRCDashboard");
    Globals::inst.SetServer(Globals::server.server.c_str(), NT_DEFAULT_PORT4);

    // NT REGISTRATION
#define REGISTER_NT(ntType, topicType) FilterStore::registerNTType(ntType, topicType);

    REGISTER_NT(nt::NetworkTableType::kBoolean, TopicTypes::Boolean)
    REGISTER_NT(nt::NetworkTableType::kString, TopicTypes::String)
    REGISTER_NT(nt::NetworkTableType::kDouble, TopicTypes::Double)
    REGISTER_NT(nt::NetworkTableType::kDoubleArray, TopicTypes::DoubleArray)
    REGISTER_NT(nt::NetworkTableType::kInteger, TopicTypes::Int)

#undef REGISTER_NT

    // SENDABLE REGISTRATION
#define REGISTER_SENDABLE(typeString, topicType) FilterStore::registerSendable(typeString, topicType);

    REGISTER_SENDABLE("String Chooser", TopicTypes::SendableChooser)
    REGISTER_SENDABLE("Field2d", TopicTypes::Field2d)

#undef REGISTER_SENDABLE

    // WIDGET REGISTRATION
#define REGISTER_TYPE(topic, widget, name) Globals::typeStore->registerType(topic, widget, name);

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

#undef REGISTER_TYPE

    QSettings settings(&app);

    // settings dont exist or firstrun is true
    bool firstRun = !settings.contains("firstRun") || settings.value("firstRun").toBool();

    if (firstRun) {
        settings.setValue("firstRun", false);

        QMessageBox::information(window, "Welcome to QFRCDashboard!",
                                 "Welcome to QFRCDashboard. Ensure to check the GitHub page in the \"about\" "
                                 "tab.\n\n"
                                 "To get started, open the NT server settings tab with Alt+S and input your desired "
                                 "NetworkTables settings. Once connected, add a tab with Ctrl+T, and search "
                                 "for your widget in the New Widget tab with Alt+N.\n\n"

                                 "Note that row and column indices start AT 0! Spans, however, do not.\n\n"

                                 "Save and load at any time with Ctrl+S and Ctrl+O, or through the File Menu. "
                                 "Close tabs with Ctrl+W or through the Tab menu. Modify or delete widgets--"
                                 "or even change their fonts--by right clicking any widget.");
    }

    Globals::inst.AddListener({{""}}, nt::EventFlags::kTopic, [] (const nt::Event &event) {
        std::string topicName(event.GetTopicInfo()->name);

        if (event.Is(nt::EventFlags::kPublish)) {
            Globals::ntTopics.append(QString::fromStdString(topicName));
        } else if (event.Is(nt::EventFlags::kUnpublish)) {
            Globals::ntTopics.removeOne(QString::fromStdString(topicName));
        }
        FilterStore::filterTopics();
    });

    // if initially connected to localhost, refilter
    // to ensure sendables are captured
    QTimer::singleShot(1000, window, [] {
        if (Globals::inst.IsConnected()) FilterStore::filterTopics();
    });

    return app.exec();
}
