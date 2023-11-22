#include "ntcore_cpp.h"

#include <QApplication>
#include <QTimer>
#include <QRadioButton>

#include "MainWindow.h"
#include "Globals.h"
#include "stores/FilterStore.h"
#include "stores/TypeStore.h"

TypeStore *Globals::typeStore = new TypeStore;

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    Globals::inst.StartClient4("QFRCDashboard");
    Globals::inst.SetServer(Globals::server.server.c_str(), NT_DEFAULT_PORT4);

    // NT REGISTRATION
#define REGISTER_NT(ntType, topicType) FilterStore::registerNTType(ntType, topicType);

    REGISTER_NT(nt::NetworkTableType::kBoolean, TopicTypes::Boolean)
    REGISTER_NT(nt::NetworkTableType::kString, TopicTypes::String)
    REGISTER_NT(nt::NetworkTableType::kDouble, TopicTypes::Double)
    REGISTER_NT(nt::NetworkTableType::kInteger, TopicTypes::Int)

#undef REGISTER_NT

    // SENDABLE REGISTRATION
#define REGISTER_SENDABLE(typeString, topicType) FilterStore::registerSendable(typeString, topicType);

    REGISTER_SENDABLE("String Chooser", TopicTypes::SendableChooser)

#undef REGISTER_SENDABLE

    // WIDGET REGISTRATION
#define REGISTER_TYPE(topic, widget, name) Globals::typeStore->registerType(topic, widget, name);

    REGISTER_TYPE(TopicTypes::Boolean, WidgetTypes::BooleanCheckbox, "Checkbox")
    REGISTER_TYPE(TopicTypes::Boolean, WidgetTypes::BooleanDisplay, "Color Display")

    REGISTER_TYPE(TopicTypes::Double, WidgetTypes::DoubleDial, "Dial")
    REGISTER_TYPE(TopicTypes::Double, WidgetTypes::DoubleDisplay, "Double Display")

    REGISTER_TYPE(TopicTypes::String, WidgetTypes::StringDisplay, "Text Display")
    REGISTER_TYPE(TopicTypes::String, WidgetTypes::EnumWidget, "Enum")

    REGISTER_TYPE(TopicTypes::Int, WidgetTypes::IntegerDisplay, "Integer Display")
    REGISTER_TYPE(TopicTypes::Int, WidgetTypes::IntegerDial, "Dial")

    REGISTER_TYPE(TopicTypes::SendableChooser, WidgetTypes::SendableChooser, "Sendable Chooser");

#undef REGISTER_TYPE

    MainWindow *window = new MainWindow();

    window->show();

    Globals::inst.AddListener({{""}}, nt::EventFlags::kTopic, [] (const nt::Event &event) {
        std::string topicName(event.GetTopicInfo()->name);

        if (event.Is(nt::EventFlags::kPublish)) {
            Globals::ntTopics.append(QString::fromStdString(topicName));
        } else if (event.Is(nt::EventFlags::kUnpublish)) {
            Globals::ntTopics.removeOne(QString::fromStdString(topicName));
        }
    });

    QTimer *filterTimer = new QTimer(window);
    QObject::connect(filterTimer, &QTimer::timeout, window, &FilterStore::filterTopics);
    filterTimer->start(50);

    QTimer *timer = new QTimer(window);
    QObject::connect(timer, &QTimer::timeout, window, [window]() {
        window->update();
    });
    timer->start(100);

    return app.exec();
}
