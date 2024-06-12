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
            QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::StandardButton::Ok | QDialogButtonBox::StandardButton::Cancel);

            listLayout->addWidget(list);
            listLayout->addWidget(box);

            constructNewWidgetList(list, listDialog);

            listDialog->open();
        }
    });

    m_menubar->addAction(newWidgetAction);

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
