#include <ntcore.h>
#include "cscore.h"

#include "ntcore_cpp.h"
#include "ntcore_cpp_types.h"

#include <QApplication>
#include <QTimer>
#include <QRadioButton>

#include "MainWindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    NT_Inst inst = nt::GetDefaultInstance();

    nt::StartClient4(inst, "QFRCDashboard");
    nt::SetServer(inst, "10.40.28.2", NT_DEFAULT_PORT4);

    MainWindow *window = new MainWindow();

    window->show();

    // QTimer *timer = new QTimer(window);
    // QWidget::connect(timer, &QTimer::timeout, diagnostics, [diagnostics] {
    //     diagnostics->updateData();
    // });
    // timer->start(20);

    return app.exec();
}
