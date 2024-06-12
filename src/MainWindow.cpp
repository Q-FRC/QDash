#include "MainWindow.h"
#include "Globals.h"
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
#include <QJsonArray>
#include <QFileDialog>

MainWindow::MainWindow()
{
    setCentralWidget(m_centralWidget = new QTabWidget);

    m_toolbar = new QToolBar(this);

    addToolBar(Qt::ToolBarArea::BottomToolBarArea, m_toolbar);

    m_menubar = menuBar();
    // NT Settings
    {
        QAction *ntServerAction = new QAction("NT Server");
        connect(ntServerAction, &QAction::triggered, this, &MainWindow::ntSettings);

        m_menubar->addAction(ntServerAction);
    } // End NT Settings

    { // File
        QMenu *fileMenu = new QMenu("File");

        QAction *saveAction = new QAction("Save");
        saveAction->setShortcut(tr("Ctrl+S"));
        fileMenu->addAction(saveAction);

        connect(saveAction, &QAction::triggered, this, &MainWindow::save);

        QAction *saveAsAction = new QAction("Save As...");
        saveAsAction->setShortcut(tr("Ctrl+Shift+S"));
        fileMenu->addAction(saveAsAction);

        connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

        QAction *loadAction = new QAction("Open File...");
        loadAction->setShortcut(tr("Ctrl+O"));
        fileMenu->addAction(loadAction);

        connect(loadAction, &QAction::triggered, this, &MainWindow::open);

        m_menubar->addMenu(fileMenu);
    } // End File

    { // Tab
        QMenu *tabMenu = new QMenu("Tab");

        QAction *newTab = new QAction("New Tab");

        newTab->setShortcut(QKeySequence::AddTab);
        connect(newTab, &QAction::triggered, this, &MainWindow::newTab);

        tabMenu->addAction(newTab);

        QAction *closeTab = new QAction("Close Tab");

        closeTab->setShortcut(QKeySequence::Close);
        connect(closeTab, &QAction::triggered, this, &MainWindow::closeTab);

        tabMenu->addAction(closeTab);

        m_menubar->addMenu(tabMenu);
    } // End Tab

    { // New Widget
        QAction *newWidgetAction = new QAction("&New Widget");

        connect(newWidgetAction, &QAction::triggered, this, &MainWindow::newWidgetPopup);

        m_menubar->addAction(newWidgetAction);
    } // End New Widget

    { // Camera View
        QAction *cameraAction = new QAction("New &Camera View");

        connect(cameraAction, &QAction::triggered, this, &MainWindow::newCameraView);

        m_menubar->addAction(cameraAction);
    } // End Camera View

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

                for (int i = 0; i < data.row + data.rowSpan; ++i) {
                    tabWidget->layout()->setRowStretch(i, 1);
                }

                for (int i = 0; i < data.col + data.colSpan; ++i) {
                    tabWidget->layout()->setColumnStretch(i, 1);
                }
            }
        }
    } 
    
    m_needsRelay = false;

    repaint();

    setWindowTitle("QFRCDashboard (" + Globals::server + ") - " + (Globals::inst.IsConnected() ? "" : "Not ") + "Connected");
}

/* File I/O */

QJsonDocument MainWindow::saveObject() {
    QJsonDocument doc{};
    QJsonArray topArray{};

    for (TabWidget *tab : m_tabWidgets) {
        QJsonObject object{};
        int tabIdx = m_centralWidget->indexOf(tab);

        object.insert("tabIdx", tabIdx);
        object.insert("tabName", m_centralWidget->tabText(tabIdx));

        QJsonArray widgets{};

        QMapIterator<BaseWidget *, WidgetData> iterator(widgetsForTab(tabIdx));

        while (iterator.hasNext()) {
            iterator.next();

            QJsonObject widgetObj = iterator.key()->saveObject();
            WidgetData data = iterator.value();

            widgetObj.insert("geometry", QJsonArray({data.row, data.col, data.rowSpan, data.colSpan}));

            widgets.insert(widgets.size(), widgetObj);
        }

        object.insert("widgets", widgets);
        topArray.insert(topArray.size(), object);
    }

    doc.setArray(topArray);
    return doc;
}

void MainWindow::loadObject(const QJsonDocument &doc) {
    QJsonArray array = doc.array();

    for (QJsonValueRef ref : array) {
        QJsonObject object = ref.toObject();

        TabWidget *tab = new TabWidget(QPoint(3,3));

        int tabIdx = object.value("tabIdx").toInt();

        m_centralWidget->insertTab(tabIdx, tab, object.value("tabName").toString());
        m_tabWidgets.insert(tabIdx, tab);

        QJsonArray widgets = object.value("widgets").toArray();

        for (QJsonValueRef wref : widgets) {
            QJsonObject widgetObject = wref.toObject();

            auto widgetData = BaseWidget::fromJson(widgetObject, tabIdx);

            BaseWidget *widget = widgetData.first;
            WidgetData data = widgetData.second;

            m_widgets.insert(widget, data);
        } // widgets
    } // tabs
    setNeedsRelay(true);
}

/* Private Member Functions */
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

QMap<BaseWidget *, WidgetData> MainWindow::widgetsForTab(int tabIdx) {
    QMap<BaseWidget *, WidgetData> map{};

    QMapIterator<BaseWidget *, WidgetData> iterator(m_widgets);

    while(iterator.hasNext()) {
        iterator.next();

        if (iterator.value().tabIdx == tabIdx) {
            map.insert(iterator.key(), iterator.value());
        }
    }

    return map;
}

void MainWindow::setNeedsRelay(bool needsRelay) {
    m_needsRelay = needsRelay;
}

/* Slots */
void MainWindow::newWidget(BaseWidget *widget, WidgetData data) {
    data.tabIdx = m_centralWidget->currentIndex();
    m_widgets.insert(widget, data);

    m_needsRelay = true;
}

// NT Settings
void MainWindow::ntSettings() {
    bool ok;
    QString server = QInputDialog::getText(this, "NT Server Settings", "Input NT4 Server Address", QLineEdit::Normal, "", &ok);

    if (!server.isEmpty() && ok) {
        Globals::server = server;
        Globals::inst.SetServer(server.toStdString().c_str(), NT_DEFAULT_PORT4);
    }
}

// File Actions
void MainWindow::save() {
    if (m_filename.isEmpty()) {
        m_filename = QFileDialog::getSaveFileName(
            this, "Save File", QDir::homePath(), "JSON Files (*.json);;All Files (*)");
    }

    QFile file(m_filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Save Failed!", "Failed to open file for writing."
                                                    "Directory may not exist or may be read-only.",
                              QMessageBox::StandardButton::Ok);
        return;
    }

    QTextStream stream(&file);
    stream << saveObject().toJson();
    file.close();
}

void MainWindow::saveAs() {
    QFile file(QFileDialog::getSaveFileName(
        this, "Save File", QDir::homePath(), "JSON Files (*.json);;All Files (*)"));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Save Failed!", "Failed to open file for writing."
                                                    "Directory may not exist or may be read-only.",
                              QMessageBox::StandardButton::Ok);
        return;
    }

    m_filename = file.fileName();

    QTextStream stream(&file);
    stream << saveObject().toJson();
    file.close();
}

void MainWindow::open() {
    QFile file(QFileDialog::getOpenFileName(
        this, "Open File", QDir::homePath(), "JSON Files (*.json);;All Files (*)"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Load Failed!", "Failed to open file for reading."
                                                    "Directory may not exist or may be inaccessible.",
                              QMessageBox::StandardButton::Ok);
        return;
    }

    m_filename = file.fileName();

    QTextStream stream(&file);
    QByteArray data = stream.readAll().toUtf8();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    loadObject(doc);
    file.close();
}

// Tab Actions
void MainWindow::newTab() {
    bool ok;
    QString tabName = QInputDialog::getText(this, "New Tab Name", "Input new tab name", QLineEdit::Normal, "", &ok);

    // TODO: max size
    if (!tabName.isEmpty() && ok) {
        TabWidget *tab = new TabWidget(QPoint(3, 3));

        m_tabWidgets.append(tab);
        m_centralWidget->addTab(tab, tabName);
        m_centralWidget->setCurrentWidget(tab);
    }
}

void MainWindow::closeTab() {
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
    }
}

// New Widget
void MainWindow::newWidgetPopup() {

    if (m_tabWidgets.length() == 0) {
        QMessageBox::StandardButton warning = QMessageBox::warning(this, "Cannot Add Widget", "You must select a tab before adding a widget.\nWould you like to add a tab now?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (warning == QMessageBox::StandardButton::Yes) {
            newTab();
        }
    } else {
        NewWidgetListDialog *listDialog = new NewWidgetListDialog;

        connect(listDialog, &NewWidgetListDialog::widgetReady, this, &MainWindow::newWidget);
        listDialog->open();
    }
}

// Camera View
void MainWindow::newCameraView() {
    if (m_tabWidgets.length() == 0) {
        QMessageBox::StandardButton warning = QMessageBox::warning(this, "Cannot Add Widget", "You must select a tab before adding a widget.\nWould you like to add a tab now?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (warning == QMessageBox::StandardButton::Yes) {
            newTab();
        }
    } else {
        NewCameraViewDialog *dialog = new NewCameraViewDialog;
        dialog->open();

        connect(dialog, &NewWidgetDialog::widgetReady, this, &MainWindow::newWidget);
    }
}
