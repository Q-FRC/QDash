#include "MainWindow.h"
#include "Globals.h"

#include "misc/NewWidgetTreeDialog.h"
#include "misc/NTSettingsDialog.h"
#include "misc/WidgetDialogGenerator.h"
#include "misc/TabMaxSizeDialog.h"

#include "stores/FilterStore.h"

#include <networktables/NetworkTableInstance.h>

#include <QToolBar>
#include <QMenuBar>
#include <QInputDialog>
#include <QMenu>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QJsonArray>
#include <QFileDialog>
#include <QApplication>
#include <QMetaProperty>
#include <QShortcut>
#include <QDrag>

MainWindow::MainWindow()
{
    setCentralWidget(m_centralWidget = new QTabWidget(this));
    m_centralWidget->setMouseTracking(true);
    setMouseTracking(true);

    m_toolbar = new QToolBar(this);

    addToolBar(Qt::ToolBarArea::BottomToolBarArea, m_toolbar);

    m_menubar = menuBar();
    // NT Settings
    {
        QAction *ntServerAction = new QAction("NT &Server");
        connect(ntServerAction, &QAction::triggered, this, &MainWindow::ntSettingsPopup);

        m_menubar->addAction(ntServerAction);
    } // End NT Settings

    { // File
        QMenu *fileMenu = new QMenu("&File", this);

        QAction *saveAction = new QAction("Save", fileMenu);
        saveAction->setShortcut(tr("Ctrl+S"));
        fileMenu->addAction(saveAction);

        connect(saveAction, &QAction::triggered, this, &MainWindow::save);

        QAction *saveAsAction = new QAction("Save As...", fileMenu);
        saveAsAction->setShortcut(tr("Ctrl+Shift+S"));
        fileMenu->addAction(saveAsAction);

        connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

        QAction *loadAction = new QAction("Open File...", fileMenu);
        loadAction->setShortcut(tr("Ctrl+O"));
        fileMenu->addAction(loadAction);

        connect(loadAction, &QAction::triggered, this, &MainWindow::openDialog);

        m_menubar->addMenu(fileMenu);
    } // End File

    { // Tab
        QMenu *tabMenu = new QMenu("&Tab", this);

        QAction *newTab = new QAction("New Tab", tabMenu);
        newTab->setShortcut(QKeySequence::AddTab);
        connect(newTab, &QAction::triggered, this, &MainWindow::newTab);
        tabMenu->addAction(newTab);

        QAction *closeTab = new QAction("Close Tab", tabMenu);
        closeTab->setShortcut(QKeySequence::Close);
        connect(closeTab, &QAction::triggered, this, &MainWindow::closeTab);
        tabMenu->addAction(closeTab);

        QAction *resizeTab = new QAction("Resize Tab", tabMenu);
        connect(resizeTab, &QAction::triggered, this, &MainWindow::setMaxSize);
        tabMenu->addAction(resizeTab);

        QAction *renameTab = new QAction("Rename Tab", tabMenu);
        connect(renameTab, &QAction::triggered, this, &MainWindow::renameTab);
        tabMenu->addAction(renameTab);

        m_menubar->addMenu(tabMenu);
    } // End Tab

    { // New Widget
        QAction *newWidgetAction = new QAction("&New Widget", this);

        connect(newWidgetAction, &QAction::triggered, this, &MainWindow::newWidgetPopup);

        m_menubar->addAction(newWidgetAction);
    } // End New Widget

    { // Standalone Widgets
        QMenu *standaloneMenu = new QMenu("New Standa&lone Widget", this);

        QAction *cameraAction = new QAction("Camera View", standaloneMenu);
        connect(cameraAction, &QAction::triggered, this, [this] {
            makeNewWidget(WidgetTypes::CameraView);
        });
        standaloneMenu->addAction(cameraAction);

        QAction *graphAction = new QAction("Graph", standaloneMenu);
        connect(graphAction, &QAction::triggered, this, [this] {
            makeNewWidget(WidgetTypes::Graph);
        });
        standaloneMenu->addAction(graphAction);

        m_menubar->addMenu(standaloneMenu);
    } // End Standalone Widgets

    { // About
        QMenu *aboutMenu = new QMenu("&About");

        QAction *aboutAction = new QAction("About QFRCDashboard", aboutMenu);
        connect(aboutAction, &QAction::triggered, this, &MainWindow::aboutDialog);
        aboutMenu->addAction(aboutAction);

        QAction *aboutQtAction = new QAction("About Qt", aboutMenu);
        connect(aboutQtAction, &QAction::triggered, this, [this] {
            QMessageBox::aboutQt(this, "About Qt");
        });
        aboutMenu->addAction(aboutQtAction);

        menuBar()->addMenu(aboutMenu);
    }

    // Initialize Shortcuts
    {
        new QShortcut(QKeySequence(Qt::Key_Control + Qt::Key_Tab), this, [this] {
            int tabIdx = m_centralWidget->currentIndex() + 1;
            if (tabIdx == m_centralWidget->count()) tabIdx = 0;

            m_centralWidget->setCurrentIndex(tabIdx);
        });
    }

    update();
}

MainWindow::~MainWindow() {}

TabWidget *MainWindow::currentTab() {
    return m_tabs.at(m_centralWidget->currentIndex());
}

/* File I/O */

QJsonDocument MainWindow::saveObject() {
    QJsonDocument doc{};
    QJsonObject topObject{};
    QJsonArray tabs{};

    for (TabWidget *tab : m_tabs) {
        QJsonObject object = tab->saveObject();
        tabs.append(object);
    }

    QJsonObject serverObj{};
    serverObj.insert("useTeamNumber", Globals::server.teamNumber);
    serverObj.insert("address", QString::fromStdString(Globals::server.server));
    serverObj.insert("port", Globals::server.port);

    topObject.insert("server", serverObj);
    topObject.insert("tabs", tabs);
    doc.setObject(topObject);
    return doc;
}

void MainWindow::loadObject(const QJsonDocument &doc) {
    QJsonObject object = doc.object();

    // TODO: load server

    QJsonObject serverObj = object.value("server").toObject();

    ServerData server = ServerData{
        serverObj.value("useTeamNumber").toBool(false),
        serverObj.value("address").toString("0.0.0.0").toStdString(),
        serverObj.value("port").toInt(NT_DEFAULT_PORT4)
    };

    setNtSettings(server);

    QJsonArray array = object.value("tabs").toArray();

    for (QJsonValueRef ref : array) {
        QJsonObject object = ref.toObject();

        TabWidget *tab = new TabWidget(QPoint(3,3), this);
        tab->setMouseTracking(true);

        tab->loadObject(object);
        m_centralWidget->addTab(tab, tab->name());
        m_tabs.append(tab);
    } // tabs
}

/* Private Member Functions */
void MainWindow::makeNewWidget(WidgetTypes type) {
    if (m_tabs.length() == 0) {
        QMessageBox::StandardButton warning = QMessageBox::warning(this, "Cannot Add Widget", "You must select a tab before adding a widget.\nWould you like to add a tab now?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (warning == QMessageBox::StandardButton::Yes) {
            newTab();
        }
    } else {
        BaseWidget *widget = BaseWidget::defaultWidgetFromTopic("", type);
        WidgetData data{0, 0, 1, 1};

        beginNewWidgetDrag(widget, data);
    }
}

/* Slots */

// NT Settings
void MainWindow::ntSettingsPopup() {
    NTSettingsDialog *dialog = new NTSettingsDialog(this);

    dialog->show();

    connect(dialog, &NTSettingsDialog::dataReady, this, &MainWindow::setNtSettings);
}

void MainWindow::setNtSettings(ServerData data) {
    std::string server = data.server;
    bool isTeamNumber = data.teamNumber;
    int port = data.port;

    if (server.empty()) return;

    if (isTeamNumber) {
        int team;
        try {
            team = std::stoi(server);
        } catch (std::invalid_argument const &) {
            QMessageBox::critical(this, "Malformed Input", "Team number was selected but address does not resemble an integer.");
            return;
        }

        Globals::inst.SetServerTeam(team, port);
    } else {
        Globals::inst.SetServer(server.c_str(), port);
    }

    Globals::server = data;
    Globals::inst.Disconnect();
}

// File Actions
void MainWindow::save() {
    if (m_filename.isEmpty()) {
        return saveAs();
    }

    QFile file(m_filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Save Failed!", "Failed to open file for writing. "
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
        QMessageBox::critical(this, "Save Failed!", "Failed to open file for writing. "
                                                    "Directory may not exist or may be read-only.",
                              QMessageBox::StandardButton::Ok);
        return;
    }

    m_filename = file.fileName();

    QTextStream stream(&file);
    stream << saveObject().toJson();
    file.close();
}

void MainWindow::openDialog() {
    QFile file(QFileDialog::getOpenFileName(
        this, "Open File", QDir::homePath(), "JSON Files (*.json);;All Files (*)"));

    open(file);
}

void MainWindow::open(QFile &file) {
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Load Failed!", "Failed to open file for reading. "
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

    if (!tabName.isEmpty() && ok) {
        TabWidget *tab = new TabWidget(QPoint(3, 3), this);
        tab->setMouseTracking(true);

        m_tabs.append(tab);
        m_centralWidget->addTab(tab, tabName);
        m_centralWidget->setCurrentWidget(tab);
        tab->setName(tabName);
    }
}

void MainWindow::closeTab() {
    if (m_tabs.empty()) return;
    int index = m_centralWidget->currentIndex();

    QMessageBox::StandardButton close = QMessageBox::question(this, "Close Tab?", "Are you sure you want to close this tab?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (close == QMessageBox::Yes) {
        TabWidget *tab = m_tabs.at(index);
        m_centralWidget->removeTab(index);
        m_tabs.remove(index);
        delete tab;
    }
}

void MainWindow::renameTab() {
    if (m_tabs.empty()) return;

    bool ok;
    QString tabName = QInputDialog::getText(this, "Tab Name", "Input new tab name", QLineEdit::Normal, m_centralWidget->tabText(m_centralWidget->currentIndex()), &ok);

    if (!tabName.isEmpty() && ok) {
        m_centralWidget->setTabText(m_centralWidget->currentIndex(), tabName);
        currentTab()->setName(tabName);
    }
}

void MainWindow::setMaxSize() {
    if (m_tabs.empty()) return;

    TabWidget *tab = currentTab();
    TabMaxSizeDialog *dialog = new TabMaxSizeDialog(this, tab->maxSize());
    dialog->show();

    connect(dialog, &TabMaxSizeDialog::dataReady, this, [this, tab](QPoint point) {
        tab->setMaxSize(point);
    });
}

// New Widget
void MainWindow::newWidgetPopup() {

    if (m_tabs.length() == 0) {
        QMessageBox::StandardButton warning = QMessageBox::warning(this, "Cannot Add Widget", "You must select a tab before adding a widget.\nWould you like to add a tab now?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (warning == QMessageBox::StandardButton::Yes) {
            newTab();
        }
    } else {
        NewWidgetTreeDialog *listDialog = new NewWidgetTreeDialog(false, this);
        listDialog->setWindowTitle("Select Widget...");
        listDialog->constructList(FilterStore::FilteredTopics);

        // Width: 1/2 of available space
        // Height: 1/2 of available space
        QRect screenSize = qApp->primaryScreen()->geometry();
        listDialog->resize(screenSize.width() / 2., screenSize.height() / 2.);

        connect(listDialog, &NewWidgetTreeDialog::widgetReady, this, &MainWindow::beginNewWidgetDrag, Qt::SingleShotConnection);

        listDialog->show();
    }
}

void MainWindow::configNewWidget(BaseWidget *widget, WidgetData data) {
    WidgetDialogGenerator *dialog = new WidgetDialogGenerator(widget, this, false, data);
    dialog->setWindowTitle("New Widget");
    dialog->show();

    connect(dialog, &WidgetDialogGenerator::widgetReady, currentTab(), &TabWidget::addWidget);
    connect(dialog, &WidgetDialogGenerator::cancelled, currentTab(), &TabWidget::deleteWidget);
}

void MainWindow::beginNewWidgetDrag(BaseWidget *widget, WidgetData data) {
    TabWidget *tab = currentTab();

    tab->setDragData(widget, data);
    tab->dragStart(QCursor::pos(), QPoint(0, 0));

    // ensure connection only occurs once
    connect(tab, &TabWidget::dragDone, this, &MainWindow::configNewWidget, Qt::SingleShotConnection);
}

//  Menu
void MainWindow::aboutDialog() {
    QStringList aboutString;
    aboutString << "Current Version: 1.0.0"
                << "GitHub: https://github.com/binex-dsk/QFRCDashboard"
                << "Author: Carson Rueter <swurl@swurl.xyz>"
                << "Contributors: Ashley Hawkins"
                << "Copyleft 2023 Carson Rueter"
                << "Enjoy :)";
    QMessageBox::about(this, "About QFRCDashboard", aboutString.join("\n"));
}
