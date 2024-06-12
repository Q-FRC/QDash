#include "ntcore_cpp.h"

#include <QApplication>
#include <QTimer>
#include <QRadioButton>

#include "MainWindow.h"
#include "Globals.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    Globals::inst.StartClient4("QFRCDashboard");
    Globals::inst.SetServer(Globals::server.toStdString().c_str(), NT_DEFAULT_PORT4);

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
        Globals::availableTopics.clear();
        for (const QString &topic : Globals::ntTopics) {
            std::string topicName = topic.toStdString();
            Globals::TopicTypes topicType;

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
                topicType = Globals::TopicTypes::SendableChooser;

                bool containsSupertable = Globals::availableTopics.contains(supertable);

                Globals::availableTopics.insert(supertable, topicType);
            } else {
                switch (entry.GetType()) {
                case nt::NetworkTableType::kBoolean: {
                    topicType = Globals::TopicTypes::Boolean;
                    break;
                }
                case nt::NetworkTableType::kDouble: {
                    topicType = Globals::TopicTypes::Double;
                    break;
                }
                case nt::NetworkTableType::kString:
                default: {
                    topicType = Globals::TopicTypes::String;
                    break;
                }
                }

                Globals::availableTopics.insert(QString::fromStdString(topicName), topicType);
            }
        }
    });
    filterTimer->start(50);

    QTimer *timer = new QTimer(window);
    QObject::connect(timer, &QTimer::timeout, window, [window]() {
        window->update();
    });
    timer->start(100);

    QObject::connect(&app, &QApplication::aboutToQuit, window, [window] {
        qDebug() << window->saveObject().toJson().toStdString();
    });

    return app.exec();
}
