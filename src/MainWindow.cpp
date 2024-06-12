#include "MainWindow.h"
#include "Globals.h"
#include "dialogs/ResizeDialog.h"
#include "dialogs/NewWidgetDialog.h"
#include "dialogs/NewCameraViewDialog.h"

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
            QListWidget *list = new QListWidget(this);
            QDialog *listDialog = new QDialog;
            QVBoxLayout *listLayout = new QVBoxLayout(listDialog);
            QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::StandardButton::Cancel);

            connect(box, &QDialogButtonBox::rejected, listDialog, &QDialog::close);

            listLayout->addWidget(list);
            listLayout->addWidget(box);

            // TODO: Separate this logic into a separate class.
            constructNewWidgetList(list, listDialog);

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
        QMenu *menu = new QMenu(this);

        QAction *resizeAction = new QAction("Resize", menu);

        menu->addAction(resizeAction);

        connect(resizeAction, &QAction::triggered, this, [this, widgetPressed](bool) {
            ResizeDialog *dialog = new ResizeDialog(m_widgets.value(widgetPressed));
            dialog->show();

            connect(dialog, &ResizeDialog::finished, this, [this, widgetPressed](WidgetData data) {
                data.tabIdx = m_centralWidget->currentIndex();

                m_widgets.remove(widgetPressed);
                m_widgets.insert(widgetPressed, data);

                setNeedsRelay(true);
            });
        });

        QAction *deleteAction = new QAction("Delete Widget", menu);
        menu->addAction(deleteAction);

        connect(deleteAction, &QAction::triggered, this, [this, widgetPressed](bool) {
            m_widgets.remove(widgetPressed);
            m_needsRelay = true;
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

void MainWindow::showNewWidgetDialog(NewWidgetDialog::WidgetTypes widgetType, std::string topic) {
    NewWidgetDialog *dialog = NewWidgetDialog::fromWidgetType(widgetType, topic);
    dialog->show();

    connect(dialog, &NewWidgetDialog::widgetReady, this, &MainWindow::newWidget);
}

// TODO: Separate this logic into a separate class.
void MainWindow::constructNewWidgetList(QListWidget *list, QDialog *dialog) {
    list->clear();
    QMapIterator<QString, Globals::TopicTypes> iterator(Globals::availableTopics);

    while (iterator.hasNext())
    {
        iterator.next();
        QString topicName = iterator.key();
        Globals::TopicTypes topicType = iterator.value();

        list->addItem(topicName);

        switch(topicType) {
        case Globals::TopicTypes::Boolean: {
            QMenu *boolMenu = new QMenu(topicName);

            QAction *checkboxAction = new QAction("Checkbox", this);
            boolMenu->addAction(checkboxAction);

            connect(checkboxAction, &QAction::triggered, this, [this, topicName, dialog](bool) {
                showNewWidgetDialog(NewWidgetDialog::WidgetTypes::BooleanCheckbox, topicName.toStdString());
                dialog->close();
            });

            QAction *colorAction = new QAction("Color Display", this);
            boolMenu->addAction(colorAction);

            connect(colorAction, &QAction::triggered, this, [this, topicName, dialog](bool) {
                showNewWidgetDialog(NewWidgetDialog::WidgetTypes::BooleanDisplay, topicName.toStdString());
                dialog->close();
            });

            connect(list, &QListWidget::itemActivated, this, [this, topicName, boolMenu](QListWidgetItem *item) {
                if (item->text() == topicName) {
                    boolMenu->popup(QCursor::pos());
                }
            });
            break;
        }
        case Globals::TopicTypes::Double: {
            QMenu *doubleMenu = new QMenu(topicName);

            QAction *displayAction = new QAction("Number Display", this);
            doubleMenu->addAction(displayAction);

            connect(displayAction, &QAction::triggered, this, [this, topicName, dialog](bool) {
                showNewWidgetDialog(NewWidgetDialog::WidgetTypes::DoubleDisplay, topicName.toStdString());
                dialog->close();
            });

            QAction *dialAction = new QAction("Dial", this);
            doubleMenu->addAction(dialAction);

            connect(dialAction, &QAction::triggered, this, [this, topicName, dialog](bool) {
                showNewWidgetDialog(NewWidgetDialog::WidgetTypes::DoubleDial, topicName.toStdString());
                dialog->close();
            });

            connect(list, &QListWidget::itemActivated, this, [this, topicName, doubleMenu](QListWidgetItem *item) {
                if (item->text() == topicName) {
                    doubleMenu->popup(QCursor::pos());
                }
            });
            break;
        }
        case Globals::TopicTypes::SendableChooser: {
            connect(list, &QListWidget::itemActivated, this, [this, topicName, dialog](QListWidgetItem *item) {
                if (item->text() == topicName) {
                    showNewWidgetDialog(NewWidgetDialog::WidgetTypes::SendableChooser, topicName.toStdString());
                    dialog->close();
                }
            });
            break;
        }
        case Globals::TopicTypes::String:
        default: {
            connect(list, &QListWidget::itemActivated, this, [this, topicName, dialog](QListWidgetItem *item) {
                if (item->text() == topicName) {
                    showNewWidgetDialog(NewWidgetDialog::WidgetTypes::StringDisplay, topicName.toStdString());
                    dialog->close();
                }
            });
            break;
        }
        }
    }
}
