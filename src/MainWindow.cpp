#include "MainWindow.h"
#include "Globals.h"
#include "dialogs/ResizeDialog.h"
#include "dialogs/NewWidgetDialog.h"

#include "ntcore/networktables/NetworkTableInstance.h"

#include <QToolBar>
#include <QMenuBar>
#include <QInputDialog>
#include <QMenu>
#include <QListWidget>
#include <QMenu>

MainWindow::MainWindow()
{
    setCentralWidget(m_centralWidget = new QTabWidget);

    // Use a stacked layout to have multiple widgets available to switch between
    // Using setCentralWidget is not possible, because upon switching the central widget
    // the previously set widget is destroyed.
    m_tabWidget = new TabWidget(QPoint(3, 3));

    m_centralWidget->setCurrentWidget(m_tabWidget);
    m_centralWidget->addTab(m_tabWidget, "Dashboard");

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
                    Globals::inst.SetServer(server.toStdString().c_str(), NT_DEFAULT_PORT4);
                }
            });

    m_menubar->addAction(ntServerAction);

    QMenu *newWidgetMenu = new QMenu("New Widget");
    connect(newWidgetMenu, &QMenu::aboutToShow, this, [this, newWidgetMenu] {
        newWidgetMenu->clear();
        for (int i = 0; i < Globals::availableTopics.length(); ++i) {
            QString topicName = Globals::availableTopics.at(i);
            nt::NetworkTableType topicType = Globals::inst.GetTopic(topicName.toStdString()).GetType();


            switch(topicType) {
            case nt::NetworkTableType::kBoolean: {
                QMenu *boolMenu = new QMenu(topicName, newWidgetMenu);

                QAction *checkboxAction = new QAction("Checkbox", this);
                boolMenu->addAction(checkboxAction);

                connect(checkboxAction, &QAction::triggered, [this, topicName](bool) {
                    showNewWidgetDialog(NewWidgetDialog::WidgetTypes::BooleanCheckbox, topicName.toStdString());
                });

                QAction *colorAction = new QAction("Color Display", this);
                boolMenu->addAction(colorAction);

                connect(colorAction, &QAction::triggered, [this, topicName](bool) {
                    showNewWidgetDialog(NewWidgetDialog::WidgetTypes::BooleanDisplay, topicName.toStdString());
                });

                newWidgetMenu->addMenu(boolMenu);
                break;
            }
            case nt::NetworkTableType::kDouble: {
                QAction *doubleAction = new QAction(topicName, this);
                newWidgetMenu->addAction(doubleAction);

                connect(doubleAction, &QAction::triggered, this, [this, topicName](bool) {
                    showNewWidgetDialog(NewWidgetDialog::WidgetTypes::DoubleDisplay, topicName.toStdString());
                });
                break;
            }
            case nt::NetworkTableType::kString:
            default: {
                QAction *stringAction = new QAction(topicName, this);
                newWidgetMenu->addAction(stringAction);

                connect(stringAction, &QAction::triggered, this, [this, topicName](bool) {
                    showNewWidgetDialog(NewWidgetDialog::WidgetTypes::StringDisplay, topicName.toStdString());
                });
                break;
            }
            }
        }
    });

    m_menubar->addMenu(newWidgetMenu);

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

    setWindowTitle("QFRCDashboard (" + Globals::server + ") - " + (Globals::inst.IsConnected() ? "" : "Not ") + "Connected");
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

            connect(dialog, &ResizeDialog::finished, this, [this, widgetPressed](QList<int> data) {
                QList<int> finalData({0, data[0], data[1], data[2], data[3]});
                m_widgets.remove(widgetPressed);
                m_widgets.insert(widgetPressed, finalData);

                setNeedsRelay(true);
            });
        });
    }
}

void MainWindow::newWidget(BaseWidget *widget, QList<int> data) {
    QList<int> finalData({m_centralWidget->currentIndex(), data[0], data[1], data[2], data[3]});
    m_widgets.insert(widget, finalData);

    m_needsRelay = true;
}

void MainWindow::showNewWidgetDialog(NewWidgetDialog::WidgetTypes widgetType, std::string topic) {
    NewWidgetDialog *dialog = NewWidgetDialog::fromWidgetType(widgetType, topic);
    dialog->show();

    connect(dialog, &NewWidgetDialog::widgetReady, this, &MainWindow::newWidget);
}
