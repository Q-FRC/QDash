#include "MainWindow.h"
#include "Globals.h"

#include "dialogs/NewWidgetTreeDialog.h"
#include "dialogs/NTSettingsDialog.h"
#include "dialogs/PreferencesDialog.h"
#include "dialogs/WidgetDialogGenerator.h"
#include "dialogs/TabMaxSizeDialog.h"
#include "dialogs/CameraSelectionDialog.h"

#include "stores/FilterStore.h"
#include "widgets/CameraViewWidget.h"

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

#include <BuildConfig.h>

#include "Constants.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow() : QMainWindow(), Ui::MainWindow()
{
    setupUi(this);

    // Initialize Shortcuts
    {
        new QShortcut(QKeySequence(Qt::Key_Control + Qt::Key_Tab), this, [this] {
            int tabIdx = centralwidget->currentIndex() + 1;
            if (tabIdx == centralwidget->count()) tabIdx = 0;

            centralwidget->setCurrentIndex(tabIdx);
        });
    }

    // this isn't available in the ui lol
    connect(centralwidget->tabBar(), &QTabBar::tabMoved, this, &MainWindow::moveTab);
}

MainWindow::~MainWindow() {
}

TabWidget *MainWindow::currentTab() {
    return m_tabs.at(centralwidget->currentIndex());
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
        centralwidget->addTab(tab, tab->name());
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

// Internal Stuff
void MainWindow::forceUpdateTab(int idx) {
    for (BaseWidget *widget : m_tabs.at(idx)->widgets()) {
        widget->forceUpdate();
    }
}

void MainWindow::moveTab(int from, int to) {
    m_tabs.move(from, to);
}

// Preferences
void MainWindow::preferences() {
    PreferencesDialog *dialog = new PreferencesDialog(this);

    dialog->show();

    connect(dialog, &PreferencesDialog::dataReady, this, [](QString styleSheet, bool loadRecent) {
        setAppStyleSheet(styleSheet);

        Settings::StyleSheet.setValue(styleSheet);
        Settings::LoadRecent.setValue(loadRecent);
    });
}

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

    addRecentFile(file);

    QTextStream stream(&file);
    QByteArray data = stream.readAll().toUtf8();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    loadObject(doc);
    file.close();
}

void MainWindow::refreshRecentFiles() {
    menuRecent_Files->clear();
    QStringList recentFiles = Settings::RecentFiles.value().toStringList();
    size_t i = 0;

    for (const QString &file : recentFiles) {
        ++i;
        QString actionName = QString(
                                 "&%1. %2"
                                 ).arg(QString::number(i), file);
        QAction *action = new QAction(actionName, menuRecent_Files);
        connect(action, &QAction::triggered, this, [this, file]() {
            QFile qfile(file);
            open(qfile);
        });

        menuRecent_Files->addAction(action);
    }
}

void MainWindow::addRecentFile(QFile &file) {
    QStringList recentFiles = Settings::RecentFiles.value().toStringList();

    QString fileName = file.fileName();
    int index = recentFiles.indexOf(fileName);

    if (index != -1) {
        recentFiles.move(index, 0);
    } else {
        recentFiles.prepend(fileName);
    }

    if (recentFiles.length() > 5) {
        recentFiles.removeLast();
    }

    Settings::RecentFiles.setValue(recentFiles);
    refreshRecentFiles();
}

// Tab Actions
void MainWindow::newTab() {
    bool ok;
    QString tabName = QInputDialog::getText(this, "New Tab Name", "Input new tab name", QLineEdit::Normal, "", &ok);

    if (!tabName.isEmpty() && ok) {
        TabWidget *tab = new TabWidget(QPoint(3, 3), this);
        tab->setMouseTracking(true);

        m_tabs.append(tab);
        centralwidget->addTab(tab, tabName);
        centralwidget->setCurrentWidget(tab);
        tab->setName(tabName);
    }
}

void MainWindow::closeTab() {
    if (m_tabs.empty()) return;
    int index = centralwidget->currentIndex();

    QMessageBox::StandardButton close = QMessageBox::question(this, "Close Tab?", "Are you sure you want to close this tab?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (close == QMessageBox::Yes) {
        TabWidget *tab = m_tabs.at(index);
        centralwidget->removeTab(index);
        m_tabs.remove(index);
        delete tab;
    }
}

void MainWindow::renameTab() {
    if (m_tabs.empty()) return;

    bool ok;
    QString tabName = QInputDialog::getText(this, "Tab Name", "Input new tab name", QLineEdit::Normal, centralwidget->tabText(centralwidget->currentIndex()), &ok);

    if (!tabName.isEmpty() && ok) {
        centralwidget->setTabText(centralwidget->currentIndex(), tabName);
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
    if (widget == nullptr) return;

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

    QMetaObject::Connection *doneConn = new QMetaObject::Connection;
    QMetaObject::Connection *cancelConn = new QMetaObject::Connection;
    *doneConn = connect(tab, &TabWidget::dragDone, this, [this, cancelConn, doneConn](BaseWidget *widget, WidgetData data) {
            configNewWidget(widget, data);
            disconnect(*doneConn);
            delete doneConn;

            disconnect(*cancelConn);
            delete cancelConn;
        }, Qt::SingleShotConnection);

    *cancelConn = connect(tab, &TabWidget::dragCancelled, this, [this, doneConn, cancelConn, widget](BaseWidget *draggedWidget) {
            disconnect(*doneConn);
            delete doneConn;
            disconnect(*cancelConn);
            delete cancelConn;

            delete widget;
        }, Qt::SingleShotConnection);
}

void MainWindow::newCameraView() {
    makeNewWidget(WidgetTypes::CameraView);
}

void MainWindow::newGraph() {
    makeNewWidget(WidgetTypes::Graph);
}

void MainWindow::cameraServerPopup() {
    if (m_tabs.length() == 0) {
        QMessageBox::StandardButton warning = QMessageBox::warning(this, "Cannot Add Widget", "You must select a tab before adding a widget.\nWould you like to add a tab now?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (warning == QMessageBox::StandardButton::Yes) {
            newTab();
        }
    } else {
        CameraSelectionDialog *dialog = new CameraSelectionDialog(this);
        dialog->show();

        connect(dialog, &CameraSelectionDialog::selectedCamera, this, [this](Camera camera) {
            QUrl url;
            if (camera.Urls.isEmpty() || camera.Urls.at(0).isValid()) {
                QMessageBox::critical(this,
                                      "Invalid Stream",
                                      "This camera contains an invalid or nonexistent stream "
                                      "URL. Please manually enter the correct URL if it "
                                      "exists.");
                url = QUrl(camera.Source);
            } else {
                url = camera.Urls.at(0);
            }

            CameraViewWidget *widget = new CameraViewWidget(
                QString("%1 (%2)").arg(camera.Name, camera.Source), url);
            WidgetData data{0, 0, 1, 1};

            beginNewWidgetDrag(widget, data);
        });
    }
}

//  Menu
void MainWindow::about() {
    QStringList aboutString;
    aboutString << "Current Version: " + BuildConfig.versionString()
                << "Build Platform: " + BuildConfig.BUILD_PLATFORM
                << "Build Date: " + BuildConfig.BUILD_DATE
                << "Git Repo: " + BuildConfig.GIT_REPO
                << "Author: Carson Rueter <swurl@swurl.xyz>"
                << "Contributors: Ashley Hawkins"
                << "Copyleft 2023-2024 Carson Rueter"
                << "Enjoy :)";
    QMessageBox::about(this, "About " + BuildConfig.APP_NAME, aboutString.join("\n"));
}

void MainWindow::aboutQt() {
    QMessageBox::aboutQt(this, "About Qt");
}
