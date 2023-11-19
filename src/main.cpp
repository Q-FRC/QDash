#include "ntcore_cpp.h"

#include <QApplication>
#include <QTimer>
#include <QRadioButton>

#include "MainWindow.h"
#include "Globals.h"
#include "stores/TypeStore.h"

TypeStore *Globals::typeStore = new TypeStore;

#define REGISTER_TYPE(topic, widget, name) Globals::typeStore->registerType(topic, widget, name);

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    Globals::inst.StartClient4("QFRCDashboard");
    Globals::inst.SetServer(Globals::server.server.c_str(), NT_DEFAULT_PORT4);

    REGISTER_TYPE(TopicTypes::Boolean, WidgetTypes::BooleanCheckbox, "Checkbox")
    REGISTER_TYPE(TopicTypes::Boolean, WidgetTypes::BooleanDisplay, "Color Display")

    REGISTER_TYPE(TopicTypes::Double, WidgetTypes::DoubleDial, "Dial")
    REGISTER_TYPE(TopicTypes::Double, WidgetTypes::DoubleDisplay, "Double Display")

    REGISTER_TYPE(TopicTypes::String, WidgetTypes::StringDisplay, "Text Display")
    REGISTER_TYPE(TopicTypes::String, WidgetTypes::EnumWidget, "Enum");

    REGISTER_TYPE(TopicTypes::SendableChooser, WidgetTypes::SendableChooser, "Sendable Chooser");

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
    QObject::connect(filterTimer, &QTimer::timeout, window, [] {
        for (const QString &topic : Globals::ntTopics) {
            std::string topicName = topic.toStdString();
            TopicTypes topicType;

            nt::NetworkTableEntry entry = Globals::inst.GetEntry(topicName);

            QStringList split = QString::fromStdString(topicName).split('/');
            QString supertable = split.sliced(0, split.length() - 1).join('/');

            nt::NetworkTableEntry typeEntry;
            std::shared_ptr<nt::NetworkTable> table = Globals::inst.GetTable(supertable.toStdString());

            if (!supertable.isEmpty()) {
                typeEntry = table->GetEntry(".type");
            } else {
                typeEntry = Globals::inst.GetEntry("");
            }

            std::string value = typeEntry.GetString("");

            if (!value.empty() && value.compare("String Chooser") == 0) {
                topicType = TopicTypes::SendableChooser;

                bool containsSupertable = Globals::availableTopics.contains(supertable);

                Globals::availableTopics.insert(supertable, topicType);
            } else {
                switch (entry.GetType()) {
                case nt::NetworkTableType::kBoolean: {
                    topicType = TopicTypes::Boolean;
                    break;
                }
                case nt::NetworkTableType::kDouble: {
                    topicType = TopicTypes::Double;
                    break;
                }
                case nt::NetworkTableType::kString:
                default: {
                    topicType = TopicTypes::String;
                    break;
                }
                }

                Globals::availableTopics.insert(QString::fromStdString(topicName), topicType);
            }

            entry.Unpublish();
        }

        // delete unused topics
        for (const QString &topic : Globals::availableTopics.keys()) {
            QStringList split = topic.split('/');
            QString supertable = split.sliced(0, split.length() - 1).join('/');
            // account for sendables
            if ((!Globals::ntTopics.contains(topic) && !Globals::ntTopics.contains(topic + "/.type")) || Globals::availableTopics.keys().contains(supertable)) {
                Globals::availableTopics.remove(topic);
            }
        }
    });
    filterTimer->start(50);

    QTimer *timer = new QTimer(window);
    QObject::connect(timer, &QTimer::timeout, window, [window]() {
        window->update();
    });
    timer->start(100);

    return app.exec();
}
