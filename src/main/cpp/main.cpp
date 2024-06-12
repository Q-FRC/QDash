#include "ntcore.h"
#include "cscore.h"

#include "ntcore_cpp.h"
#include "ntcore_cpp_types.h"

#include <QApplication>
#include <QTimer>
#include <QRadioButton>

#include "MainWindow.h"
#include "Globals.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    nt::StartClient4(Globals::inst, "QFRCDashboard");
    nt::SetServer(Globals::inst, Globals::server.toStdString().c_str(), NT_DEFAULT_PORT4);

    MainWindow *window = new MainWindow();

    window->show();

    QTimer *timer = new QTimer(window);
    QObject::connect(timer, &QTimer::timeout, [window]() {
        window->update();
    });
    timer->start(100);

    // QTimer *timer = new QTimer(window);
    // QWidget::connect(timer, &QTimer::timeout, diagnostics, [diagnostics] {
    //     diagnostics->updateData();
    // });
    // timer->start(20);

    return app.exec();
}
