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
#include <QMessageBox>

MainWindow::MainWindow()
{
    setCentralWidget(m_centralWidget = new QTabWidget);

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

    QMenu *tabMenu = new QMenu("Tab");

    QAction *newTab = new QAction("New Tab");

    newTab->setShortcut(QKeySequence::AddTab);
    connect(newTab, &QAction::triggered, this, [this](bool) {
        bool ok;
        QString tabName = QInputDialog::getText(this, "New Tab Name", "Input new tab name", QLineEdit::Normal, "", &ok);

        // TODO: max size
        if (!tabName.isEmpty() && ok) {
            TabWidget *tab = new TabWidget(QPoint(3, 3));

            m_tabWidgets.append(tab);
            m_centralWidget->addTab(tab, tabName);
            m_centralWidget->setCurrentWidget(tab);
        }
    });

    tabMenu->addAction(newTab);

    QAction *closeTab = new QAction("Close Tab");

    closeTab->setShortcut(QKeySequence::Close);
    connect(closeTab, &QAction::triggered, this, [this](bool) {
        int index = m_centralWidget->currentIndex();

        QMessageBox::StandardButton close = QMessageBox::question(this, "Close Tab?", "Are you sure you want to close this tab?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (close == QMessageBox::Yes) {
            m_centralWidget->removeTab(index);
            m_tabWidgets.remove(index);
        }
    });

    tabMenu->addAction(closeTab);

    m_menubar->addMenu(tabMenu);

    QMenu *newWidgetMenu = new QMenu("&New Widget");
    connect(newWidgetMenu, &QMenu::aboutToShow, this, [this, newWidgetMenu, newTab] {
        if (m_tabWidgets.length() == 0) {
            QMessageBox::StandardButton warning = QMessageBox::warning(this, "Cannot Add Widget", "You must select a tab before adding a widget.\nWould you like to add a tab now?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (warning == QMessageBox::StandardButton::Yes) {
                newTab->trigger();
            }
        } else {
            constructNewWidgetMenu(newWidgetMenu);
            newWidgetMenu->show();
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
            TabWidget *tabWidget = m_tabWidgets.at(data.at(0));

            if (tabWidget != nullptr) {
                tabWidget->layout()->removeWidget(iterator.key());
                tabWidget->layout()->addWidget(iterator.key(), data[1], data[2], data[3], data[4]);
            }
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
        if (iterator.key()->geometry().contains(event->pos()) && iterator.value().at(0) == m_centralWidget->currentIndex()) {
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
                QList<int> finalData({m_centralWidget->currentIndex(), data[0], data[1], data[2], data[3]});
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

void MainWindow::constructNewWidgetMenu(QMenu *menu) {
    menu->clear();
    for (int i = 0; i < Globals::availableTopics.length(); ++i) {
        QString topicName = Globals::availableTopics.at(i);
        nt::NetworkTableType topicType = Globals::inst.GetTopic(topicName.toStdString()).GetType();

        switch(topicType) {
        case nt::NetworkTableType::kBoolean: {
            QMenu *boolMenu = new QMenu(topicName, menu);

            QAction *checkboxAction = new QAction("Checkbox", this);
            boolMenu->addAction(checkboxAction);

            connect(checkboxAction, &QAction::triggered, this, [this, topicName](bool) {
                showNewWidgetDialog(NewWidgetDialog::WidgetTypes::BooleanCheckbox, topicName.toStdString());
            });

            QAction *colorAction = new QAction("Color Display", this);
            boolMenu->addAction(colorAction);

            connect(colorAction, &QAction::triggered, this, [this, topicName](bool) {
                showNewWidgetDialog(NewWidgetDialog::WidgetTypes::BooleanDisplay, topicName.toStdString());
            });

            menu->addMenu(boolMenu);
            break;
        }
        case nt::NetworkTableType::kDouble: {
            QAction *doubleAction = new QAction(topicName, this);
            menu->addAction(doubleAction);

            connect(doubleAction, &QAction::triggered, this, [this, topicName](bool) {
                showNewWidgetDialog(NewWidgetDialog::WidgetTypes::DoubleDisplay, topicName.toStdString());
            });
            break;
        }
        case nt::NetworkTableType::kString:
        default: {
            QAction *stringAction = new QAction(topicName, this);
            menu->addAction(stringAction);

            connect(stringAction, &QAction::triggered, this, [this, topicName](bool) {
                showNewWidgetDialog(NewWidgetDialog::WidgetTypes::StringDisplay, topicName.toStdString());
            });
            break;
        }
        }
    }
}
