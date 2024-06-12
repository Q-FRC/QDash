#include <ntcore.h>
#include "cscore.h"

#include "ntcore_cpp.h"
#include "ntcore_cpp_types.h"

#include <QApplication>
#include <QTimer>
#include <QRadioButton>

#include "MainWindow.h"
#include "RobotDiagnostics.h"
#include "PitChecklist.h"
#include "EventData.h"

NT_Subscriber coolSub;

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    NT_Inst inst = nt::GetDefaultInstance();

    nt::StartClient4(inst, "Pit Display");
    nt::SetServer(inst, "10.40.28.2", NT_DEFAULT_PORT4);

    RobotDiagnostics *diagnostics = new RobotDiagnostics(inst);
    PitChecklist *checklist = new PitChecklist();
    EventData *eventData = new EventData();

    MainWindow *window = new MainWindow(diagnostics, checklist, eventData);

    window->show();

    QTimer *timer = new QTimer(window);
    QWidget::connect(timer, &QTimer::timeout, diagnostics, [diagnostics] {
        diagnostics->updateData();
    });
    timer->start(20);

    QTimer *networkTimer = new QTimer(window);
    QWidget::connect(networkTimer, &QTimer::timeout, eventData, [eventData] {
        eventData->updateRankingData();
        eventData->updateEventData();
    });
    networkTimer->start(4000);

    return app.exec();
}
