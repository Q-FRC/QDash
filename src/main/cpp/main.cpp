#include <ntcore.h>
#include "cscore.h"

#include "ntcore_cpp.h"
#include "ntcore_cpp_types.h"

#include <QMainWindow>
#include <QLabel>
#include <QApplication>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QTimer>

NT_Subscriber coolSub;

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    NT_Inst inst = nt::GetDefaultInstance();

    nt::StartClient4(inst, "deez");
    nt::SetServer(inst, "*", NT_DEFAULT_PORT4);

    coolSub = nt::Subscribe(nt::GetTopic(inst, "/SmartDashboard/bruh"), NT_STRING, "string");

    QMainWindow *window = new QMainWindow();

    QWidget *widget = new QWidget(window);

    QVBoxLayout *layout = new QVBoxLayout(widget);

    QLabel *label = new QLabel(widget);
    label->setText("Hello :)");

    layout->addWidget(label);

    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Close, widget);
    QWidget::connect(box, &QDialogButtonBox::rejected, window, []() {
        qApp->quit();
    });

    layout->addWidget(box);

    widget->setLayout(layout);
    window->setCentralWidget(widget);

    window->show();

    QTimer *timer = new QTimer(window);
    QWidget::connect(timer, &QTimer::timeout, window, [label, coolSub]() {
        label->setText(QString::fromStdString(nt::GetString(coolSub, "default")));
    });
    timer->start(50);

    return app.exec();
}
