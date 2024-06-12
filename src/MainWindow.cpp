#include "MainWindow.h"
#include "Globals.h"
#include "dialogs/ResizeDialog.h"
#include "dialogs/NewWidgetDialog.h"

#include <QToolBar>
#include <QMenuBar>
#include <QInputDialog>
#include <QMenu>

MainWindow::MainWindow()
{
    setCentralWidget(m_centralWidget = new QTabWidget);

    // Use a stacked layout to have multiple widgets available to switch between
    // Using setCentralWidget is not possible, because upon switching the central widget
    // the previously set widget is destroyed.
    m_tabWidget = new TabWidget(QPoint(3, 3));

    BooleanDisplayWidget *boolean = new BooleanDisplayWidget("Test", "yoooo", "cryaboutit");
    boolean->setTrueColor(Qt::blue);
    boolean->setFalseColor(Qt::yellow);

    boolean->update();

    NumberDisplayWidget *number = new NumberDisplayWidget("Numero", 25.3, "numero");
    StringDisplayWidget *string = new StringDisplayWidget("String", "hola", "stringio");

    m_centralWidget->setCurrentWidget(m_tabWidget);
    m_centralWidget->addTab(m_tabWidget, "Dashboard");

    QList<int> booleanData({0, 0, 0, 1, 1});
    m_widgets.insert(boolean, booleanData);

    QList<int> numberData({0, 0, 1, 2, 1});
    m_widgets.insert(number, numberData);

    QList<int> stringData({0, 1, 0, 1, 1});
    m_widgets.insert(string, stringData);

    m_toolbar = new QToolBar(this);

    addToolBar(Qt::ToolBarArea::BottomToolBarArea, m_toolbar);

    m_menubar = menuBar();

    QAction *ntServerAction = new QAction("NT Server");
    connect(ntServerAction, &QAction::triggered, this, [this](bool)
            {
                bool ok;
                QString server = QInputDialog::getText(this, "NT Server Settings", "Input NT4 Server Address", QLineEdit::Normal, "", &ok);

                if (!server.isEmpty() && ok) {
                    Globals::server = server;
                    nt::SetServer(Globals::inst, Globals::server.toStdString().c_str(), NT_DEFAULT_PORT4);
                }
            });

    m_menubar->addAction(ntServerAction);

    QAction *newWidget = new QAction("New Widget");
    connect(newWidget, &QAction::triggered, this, [this](bool) {
        NewWidgetDialog *dialog = new NewWidgetDialog("cryaboutit");
        dialog->show();

        connect(dialog, &NewWidgetDialog::dataReady, this, [this, dialog](std::string topic, NT_Type type, QString name, QColor trueColor, QColor falseColor, QList<int> widgetData) {
            switch(type) {
                case NT_BOOLEAN: {
                    BooleanDisplayWidget *widget = new BooleanDisplayWidget(name, false, QString::fromStdString(topic));
                    widget->setTrueColor(trueColor);
                    widget->setFalseColor(falseColor);
                    QList<int> finalData({m_centralWidget->currentIndex(), widgetData[0], widgetData[1], widgetData[2], widgetData[3]});
                    m_widgets.insert(widget, finalData);
                    break;
                }
                case NT_DOUBLE: {
                    NumberDisplayWidget *widget = new NumberDisplayWidget(name, 0., QString::fromStdString(topic));
                    QList<int> finalData({m_centralWidget->currentIndex(), widgetData[0], widgetData[1], widgetData[2], widgetData[3]});
                    m_widgets.insert(widget, finalData);
                }
                default:
                case NT_STRING: {
                    StringDisplayWidget *widget = new StringDisplayWidget(name, "", QString::fromStdString(topic));
                    QList<int> finalData({m_centralWidget->currentIndex(), widgetData[0], widgetData[1], widgetData[2], widgetData[3]});
                    m_widgets.insert(widget, finalData);
                    break;
                }
            }
            m_needsRelay = true;
            dialog->close();
        });
    });
    m_menubar->addAction(newWidget);

    update();
}

MainWindow::~MainWindow() {}

void MainWindow::update()
{
    QMapIterator<BaseWidget *, QList<int>> iterator(m_widgets);

    while (iterator.hasNext())
    {
        iterator.next();
        iterator.key()->update();
        if (m_needsRelay) {
            QList<int> data = iterator.value();
            m_tabWidget->layout()->removeWidget(iterator.key());
            m_tabWidget->layout()->addWidget(iterator.key(), data[1], data[2], data[3], data[4]);
        }
    } 
    
    m_needsRelay = false;

    setWindowTitle("QFRCDashboard (" + Globals::server + ") - " + (nt::IsConnected(Globals::inst) ? "" : "Not ") + "Connected");
}

void MainWindow::setNeedsRelay(bool needsRelay) {
    m_needsRelay = needsRelay;
}

QList<int> MainWindow::getWidgetData(BaseWidget *widget) {
    return m_widgets.value(widget);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    BaseWidget *widgetPressed = nullptr;

    QMapIterator<BaseWidget *, QList<int>> iterator(m_widgets);

    while (iterator.hasNext())
    {
        iterator.next();
        if (iterator.key()->geometry().contains(event->pos())) {
            widgetPressed = iterator.key();
            break;
        }
    }

    if (event->button() == Qt::RightButton && widgetPressed) {
        QMenu *menu = new QMenu(this);

        QAction *resizeAction = new QAction("Resize", menu);

        menu->addAction(resizeAction);
        menu->popup(event->globalPosition().toPoint());

        connect(resizeAction, &QAction::triggered, this, [this, widgetPressed](bool) {
            ResizeDialog *dialog = new ResizeDialog(m_widgets.value(widgetPressed));
            dialog->show();

            connect(dialog, &ResizeDialog::finished, this, [this, widgetPressed, dialog](QList<int> data) {
                QList<int> finalData({0, data[0], data[1], data[2], data[3]});
                m_widgets.remove(widgetPressed);
                m_widgets.insert(widgetPressed, finalData);

                dialog->close();
                setNeedsRelay(true);
            });
        });
    }
}
