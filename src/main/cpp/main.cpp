#include <ntcore.h>
#include "cscore.h"

#include "ntcore_cpp.h"
#include "ntcore_cpp_types.h"

#include <QApplication>
#include <QTimer>

#include "MainWindow.h"

NT_Subscriber coolSub;

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    NT_Inst inst = nt::GetDefaultInstance();

    nt::StartClient4(inst, "Pit Display");
    nt::SetServer(inst, "*", NT_DEFAULT_PORT4);

    MainWindow *window = new MainWindow(inst);

    window->show();

    QTimer *timer = new QTimer(window);
    QWidget::connect(timer, &QTimer::timeout, window, [window]() {
        window->updateLabels();
    });
    timer->start(50);

    return app.exec();
}
