#include "MainWindow.h"
#include "Globals.h"
#include "misc/ResizeDialog.h"
#include "dialogs/NewWidgetDialog.h"
#include "dialogs/NewCameraViewDialog.h"

#include "misc/NewWidgetListDialog.h"

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
            QWidget *tab = m_centralWidget->widget(index);
            m_centralWidget->removeTab(index);
            m_tabWidgets.remove(index);
            QMapIterator<BaseWidget *, WidgetData> iterator(m_widgets);

            while (iterator.hasNext())
            {
                iterator.next();
                BaseWidget *widget = iterator.key();
                WidgetData data = iterator.value();
                if (data.tabIdx == index) {
                    m_widgets.remove(widget);
                } else if (data.tabIdx > index) {
                    data.tabIdx -= 1;

                    m_widgets.remove(widget);
                    m_widgets.insert(widget, data);
                }
            }

//            delete tab;
        }
    });

    tabMenu->addAction(closeTab);

    m_menubar->addMenu(tabMenu);

    QAction *newWidgetAction = new QAction("&New Widget");

    connect(newWidgetAction, &QAction::triggered, this, [this, newTab](bool) {

        if (m_tabWidgets.length() == 0) {
            QMessageBox::StandardButton warning = QMessageBox::warning(this, "Cannot Add Widget", "You must select a tab before adding a widget.\nWould you like to add a tab now?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (warning == QMessageBox::StandardButton::Yes) {
                newTab->trigger();
            }
        } else {
            NewWidgetListDialog *listDialog = new NewWidgetListDialog;

            connect(listDialog, &NewWidgetListDialog::widgetReady, this, &MainWindow::newWidget);
            listDialog->open();
        }
    });

    m_menubar->addAction(newWidgetAction);

    QAction *cameraAction = new QAction("New &Camera View");

    connect(cameraAction, &QAction::triggered, this, [this, newTab](bool) {
        if (m_tabWidgets.length() == 0) {
            QMessageBox::StandardButton warning = QMessageBox::warning(this, "Cannot Add Widget", "You must select a tab before adding a widget.\nWould you like to add a tab now?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (warning == QMessageBox::StandardButton::Yes) {
                newTab->trigger();
            }
        } else {
            NewCameraViewDialog *dialog = new NewCameraViewDialog;
            dialog->open();

            connect(dialog, &NewWidgetDialog::widgetReady, this, &MainWindow::newWidget);
        }
    });

    m_menubar->addAction(cameraAction);

    update();
}

MainWindow::~MainWindow() {}

void MainWindow::update()
{
    QMapIterator<BaseWidget *, WidgetData> iterator(m_widgets);

    while (iterator.hasNext())
    {
        iterator.next();
        iterator.key()->update();
        if (m_needsRelay) {
            WidgetData data = iterator.value();
            TabWidget *tabWidget = m_tabWidgets.at(data.tabIdx);

            if (tabWidget != nullptr) {
                tabWidget->layout()->removeWidget(iterator.key());
                tabWidget->layout()->addWidget(iterator.key(), data.row, data.col, data.rowSpan, data.colSpan);

                for (int i = 0; i < data.rowSpan; ++i) {
                    tabWidget->layout()->setRowStretch(data.row + i, 1);
                }

                for (int i = 0; i < data.colSpan; ++i) {
                    tabWidget->layout()->setColumnStretch(data.col + i, 1);
                }
            }
        }
    } 
    
    m_needsRelay = false;

    repaint();

    setWindowTitle("QFRCDashboard (" + Globals::server + ") - " + (Globals::inst.IsConnected() ? "" : "Not ") + "Connected");
}

void MainWindow::setNeedsRelay(bool needsRelay) {
    m_needsRelay = needsRelay;
}

WidgetData MainWindow::getWidgetData(BaseWidget *widget) {
    return m_widgets.value(widget);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    BaseWidget *widgetPressed = nullptr;

    QMapIterator<BaseWidget *, WidgetData> iterator(m_widgets);

    while (iterator.hasNext())
    {
        iterator.next();
        BaseWidget *widget = iterator.key();
        WidgetData data = iterator.value();
        if (widget->geometry().contains(event->pos()) && data.tabIdx == m_centralWidget->currentIndex()) {
            widgetPressed = widget;
            break;
        }
    }

    if (event->button() == Qt::RightButton && widgetPressed) {
        QMenu *menu = widgetPressed->constructContextMenu(m_widgets.value(widgetPressed));

        connect(widgetPressed, &BaseWidget::resizeRequested, this, [this, widgetPressed](WidgetData data) {
            data.tabIdx = m_centralWidget->currentIndex();

            m_widgets.remove(widgetPressed);
            m_widgets.insert(widgetPressed, data);

            setNeedsRelay(true);
        });

        connect(widgetPressed, &BaseWidget::deleteRequested, this, [this, widgetPressed] {
            m_widgets.remove(widgetPressed);
            setNeedsRelay(true);

            delete widgetPressed;
        });

        menu->popup(event->globalPosition().toPoint());
    }
}

void MainWindow::newWidget(BaseWidget *widget, WidgetData data) {
    data.tabIdx = m_centralWidget->currentIndex();
    m_widgets.insert(widget, data);

    m_needsRelay = true;
}
