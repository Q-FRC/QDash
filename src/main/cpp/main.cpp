#include <ntcore.h>
#include "cscore.h"

#include "ntcore_cpp.h"
#include "ntcore_cpp_types.h"

#include <QApplication>
#include <QTimer>

#include "MainWindow.h"
#include "RobotDiagnostics.h"
#include "PitChecklist.h"

NT_Subscriber coolSub;

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    NT_Inst inst = nt::GetDefaultInstance();

    nt::StartClient4(inst, "Pit Display");
    nt::SetServer(inst, "*", NT_DEFAULT_PORT4);

    RobotDiagnostics *diagnostics = new RobotDiagnostics(inst);
    PitChecklist *checklist = new PitChecklist();

    MainWindow *window = new MainWindow(diagnostics, checklist);

    window->show();

    QTimer *timer = new QTimer(window);
    QWidget::connect(timer, &QTimer::timeout, diagnostics, [diagnostics]() {
        diagnostics->updateData();
    });
    timer->start(50);

    return app.exec();
}
