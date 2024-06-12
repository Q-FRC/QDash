#include "ntcore.h"

#include "ntcore_cpp.h"
#include "ntcore_cpp_types.h"

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

    Globals::inst.AddListener({{"/"}}, nt::EventFlags::kTopic, [] (const nt::Event &event) {
        if (event.Is(nt::EventFlags::kPublish)) {
            qDebug() << "Published" << QString::fromStdString(event.GetTopicInfo()->name);
        }
    });

    QTimer *timer = new QTimer(window);
    QObject::connect(timer, &QTimer::timeout, [window]() {
        window->update();
    });
    timer->start(100);

    return app.exec();
}
