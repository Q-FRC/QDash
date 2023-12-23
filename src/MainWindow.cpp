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

void MainWindow::update() {
    QMainWindow::update();

    QMapIterator<BaseWidget *, WidgetData> iterator(m_widgets);

    while (iterator.hasNext())
    {
        iterator.next();

        iterator.key()->update();
    }
}

/* File I/O */

QJsonDocument MainWindow::saveObject() {
    QJsonDocument doc{};
    QJsonObject topObject{};
    QJsonArray tabs{};

    for (TabWidget *tab : m_tabWidgets) {
        QJsonObject object{};
        int tabIdx = m_centralWidget->indexOf(tab);

        object.insert("tabIdx", tabIdx);
        object.insert("tabName", m_centralWidget->tabText(tabIdx));

        QPoint maxSize = m_tabWidgets.at(tabIdx)->maxSize();
        object.insert("maxSize", QJsonArray{ maxSize.x(), maxSize.y() });

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
        tabs.insert(tabs.size(), object);
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

        int tabIdx = object.value("tabIdx").toInt();

        m_centralWidget->insertTab(tabIdx, tab, object.value("tabName").toString());
        m_tabWidgets.insert(tabIdx, tab);

        QJsonArray maxSize = object.value("maxSize").toArray();
        tab->setMaxSize(QPoint(maxSize.at(0).toInt(3), maxSize.at(1).toInt(3)));

        QJsonArray widgets = object.value("widgets").toArray();

        for (QJsonValueRef wref : widgets) {
            QJsonObject widgetObject = wref.toObject();

            QString topic = widgetObject.value("Topic").toString("");

            WidgetTypes type = (WidgetTypes) widgetObject.value("widgetType").toInt();
            BaseWidget *widget = BaseWidget::defaultWidgetFromTopic(topic, type);
            widget->setParent(this);
            widget->installEventFilter(this);

            WidgetData data = widget->fromJson(widgetObject, tabIdx);

            m_widgets.insert(widget, data);
        } // widgets
    } // tabs

    relay();
}

bool MainWindow::positionContainsWidget(WidgetData widgetData) {
    if (m_tabWidgets.empty()) return false;

    TabWidget *tab = m_tabWidgets.at(widgetData.tabIdx);
    for (int i = 0; i < widgetData.rowSpan; ++i) {
        for (int j = 0; j < widgetData.colSpan; ++j) {
            int row = widgetData.row + i;
            int col = widgetData.col + j;

            if (tab->layout()->itemAtPosition(row, col)) return true;
        }
    }

    return false;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    BaseWidget *casted = qobject_cast<BaseWidget *>(object);

    TabWidget *tabCasted = qobject_cast<TabWidget *>(object);
    if (event->type() == QEvent::MouseMove && (tabCasted != nullptr || casted != nullptr)) {
        mouseMoveEvent((QMouseEvent *)(event));
        return tabCasted != nullptr;
    }
    return false;
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
        if (data.tabIdx != m_centralWidget->currentIndex()) continue;
        TabWidget *tab = m_tabWidgets.at(data.tabIdx);

        // map to tab widget as the base widget's geometry is relative to the tab widget
        if (widget->geometry().contains(tab->mapFromGlobal(event->globalPosition()).toPoint())) {
            widgetPressed = widget;
            break;
        }
    }

    if (!widgetPressed) {
        return;
    }
    if (event->button() == Qt::RightButton) {
        QMenu *menu = widgetPressed->constructContextMenu(m_widgets.value(widgetPressed));

        connect(widgetPressed, &BaseWidget::reconfigRequested, this, [this, widgetPressed](BaseWidget *widget, WidgetData data) {
            data.tabIdx = m_centralWidget->currentIndex();

            m_widgets.remove(widgetPressed);
            m_widgets.insert(widget, data);

            widget->setTitle(widgetPressed->title());

            relay();
        });

        connect(widgetPressed, &BaseWidget::deleteRequested, this, [this, widgetPressed] {
            m_widgets.remove(widgetPressed);
            relay();

            widgetPressed->deleteLater();
        });

        menu->popup(event->globalPosition().toPoint());
        return;
    }

    if (event->button() != Qt::LeftButton) {
        return;
    }

    m_draggedWidget = widgetPressed;
    m_draggedWidgetData = m_widgets.value(widgetPressed);

    if (widgetPressed->resizing() == NONE) {
        dragStart(event->pos(), event->pos());
    } else {
        resizeStart(event->pos());
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if ((event->pos() - m_dragStart).manhattanLength()
        < QApplication::startDragDistance())
        return;
    if (m_tabWidgets.empty()) return;

    TabWidget *tab = m_tabWidgets.at(m_centralWidget->currentIndex());
    if (!tab) return;

    QPoint point = tab->mapFromGlobal(event->globalPosition()).toPoint();

    if (m_dragging) {
        dragMove(point);
    } else if (m_resizing) {
        resizeMove(point);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (m_dragging) {
        dragRelease(event->pos());
    } else if (m_resizing) {
        resizeRelease(event->pos());
    }

    m_dragging = false;
    m_resizing = false;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    update();
}

void MainWindow::dragStart(QPoint point, QPoint offset) {
    m_dragStart = point;
    m_draggedWidget->raise();
    if (offset == point) {
        m_dragOffset = (offset - m_draggedWidget->pos());
    } else {
        m_dragOffset = offset;
    }
    m_dragging = true;
}

void MainWindow::dragMove(QPoint point) {
    if (m_tabWidgets.empty()) return;

    int tabIdx = m_centralWidget->currentIndex();
    TabWidget *tab = m_tabWidgets.at(tabIdx);

    if (m_draggedWidget->isVisible()) {
        QPoint offset = (tab->mapToGlobal(point)) - m_dragOffset;
        m_draggedWidget->move(offset);

        if (!tab->hasSelection()) {
            tab->layout()->removeWidget(m_draggedWidget);
            m_widgets.remove(m_draggedWidget);
        }
    }

    int col = std::floor(point.x() / (tab->width() / tab->maxSize().x()));
    int row = std::floor(point.y() / (tab->height() / tab->maxSize().y()));

    int colSpan = m_draggedWidgetData.colSpan;
    int rowSpan = m_draggedWidgetData.rowSpan;

    WidgetData data{tabIdx, row, col, rowSpan, colSpan};

    tab->setValidSelection(!positionContainsWidget(data) &&
                           (row + rowSpan - 1 < tab->maxSize().x() && (col + colSpan - 1) < tab->maxSize().y()));

    tab->setSelectedIndex(data);
}

void MainWindow::dragRelease(QPoint point) {
    if (m_tabWidgets.empty()) return;
    TabWidget *tab = m_tabWidgets.at(m_centralWidget->currentIndex());

    if (!tab->hasSelection()) return;

    WidgetData data;

    if (tab->isValidSelection()) {
        data = tab->selectedIndex();
    } else {
        data = m_draggedWidgetData;
    }

    m_widgets.insert(m_draggedWidget,
                     data);
    relay();
    tab->setHasSelection(false);

    m_draggedWidget = nullptr;
    m_dragging = false;

    emit dragDone(m_draggedWidget, data);
}

void MainWindow::resizeStart(QPoint point) {
    m_dragStart = point;
    m_draggedWidget->raise();
    m_currentResize = m_draggedWidget->resizing();
    m_initialSize = m_draggedWidget->geometry();

    setCursor(m_draggedWidget->cursor());
    m_resizing = true;
}

void MainWindow::resizeRelease(QPoint point) {
    if (m_tabWidgets.empty()) return;
    TabWidget *tab = m_tabWidgets.at(m_centralWidget->currentIndex());

    if (!tab->hasSelection()) return;

    WidgetData data;

    if (tab->isValidSelection()) {
        data = tab->selectedIndex();
    } else {
        data = m_draggedWidgetData;
    }

    m_widgets.insert(m_draggedWidget,
                     data);
    relay();
    tab->setHasSelection(false);

    m_resizing = false;
    m_draggedWidget = nullptr;
}

void MainWindow::resizeMove(QPoint point) {
    if (m_initialSize == QRect() || m_tabWidgets.empty())
        return;

    int tabIdx = m_centralWidget->currentIndex();
    TabWidget *tab = m_tabWidgets.at(tabIdx);

    if (!tab->hasSelection()) {
        tab->layout()->removeWidget(m_draggedWidget);
        m_widgets.remove(m_draggedWidget);
    }

    QPoint offset = tab->mapToGlobal(point) - m_dragStart;
    ResizeDirection dir = m_currentResize;

    int dx = 0;
    int dy = 0;
    int dh = 0;
    int dw = 0;

    if (dir & LEFT) {
        dx = offset.x();
        dw = -offset.x();
    }
    if (dir & RIGHT) {
        dw = offset.x();
    }
    if (dir & TOP) {
        dy = offset.y();
        dh = -offset.y();
    }
    if (dir & BOTTOM) {
        dh = offset.y();
    }

    int colF = std::floor(point.x() / (tab->width() / tab->maxSize().x()));
    int rowF = std::floor(point.y() / (tab->height() / tab->maxSize().y()));

    int colSpanI = m_draggedWidgetData.colSpan;
    int rowSpanI = m_draggedWidgetData.rowSpan;

    int colI = m_draggedWidgetData.col;
    int rowI = m_draggedWidgetData.row;

    if (dir & LEFT) colI += colSpanI - 1;
    if (dir & TOP) rowI += rowSpanI - 1;

    int col = dir & LEFT ? std::min(colF, colI) : colI;
    int row = dir & TOP ? std::min(rowF, rowI) : rowI;

    int colSpan = (dir & LEFT || dir & RIGHT) ? (dir & RIGHT ? std::abs(colF - colI) + 1 : std::abs(colI - colF) + 1) : colSpanI;
    int rowSpan = (dir & TOP || dir & BOTTOM) ? (dir & BOTTOM ? std::abs(rowF - rowI) + 1 : std::abs(rowI - rowF) + 1) : rowSpanI;

    if (colSpan < 1) colSpan = 1;
    if (rowSpan < 1) rowSpan = 1;

    if (dw <= -m_initialSize.width()) {
        dw = 0;
        colSpan = 1;

    }
    if (dh <= -m_initialSize.height()) {
        dh = 0;
        rowSpan = 1;
    }

    m_draggedWidget->setGeometry(
        m_initialSize.x() + dx,
        m_initialSize.y() + dy,
        m_initialSize.width() + dw,
        m_initialSize.height() + dh);

    WidgetData data{tabIdx, row, col, rowSpan, colSpan};
    tab->setSelectedIndex(data);

    tab->setValidSelection(!positionContainsWidget(data) &&
                           (row + rowSpan - 1 < tab->maxSize().x() && (col + colSpan - 1) < tab->maxSize().y()));

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

void MainWindow::relay() {
    QMapIterator<BaseWidget *, WidgetData> iterator(m_widgets);

    while (iterator.hasNext())
    {
        iterator.next();

        WidgetData data = iterator.value();
        TabWidget *tabWidget = m_tabWidgets.at(data.tabIdx);

        if (tabWidget != nullptr) {
            tabWidget->layout()->removeWidget(iterator.key());
            tabWidget->layout()->addWidget(iterator.key(), data.row, data.col, data.rowSpan, data.colSpan);
        }
    }

    update();
}

void MainWindow::makeNewWidget(WidgetTypes type) {
    if (m_tabWidgets.length() == 0) {
        QMessageBox::StandardButton warning = QMessageBox::warning(this, "Cannot Add Widget", "You must select a tab before adding a widget.\nWould you like to add a tab now?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (warning == QMessageBox::StandardButton::Yes) {
            newTab();
        }
    } else {
        BaseWidget *widget = BaseWidget::defaultWidgetFromTopic("", type);
        m_draggedWidget = widget;

        WidgetData data{m_centralWidget->currentIndex(), 0, 0, 1, 1};
        m_draggedWidgetData = data;

        dragStart(QCursor::pos(), QPoint(0, 0));

        // ensure connection only occurs once
        QObject *receiver = new QObject(this);
        connect(this, &MainWindow::dragDone, receiver, [this, receiver, widget](BaseWidget *, WidgetData data) {
            receiver->deleteLater();

            WidgetDialogGenerator *dialog = new WidgetDialogGenerator(widget, this, false, data);
            dialog->setWindowTitle("New Widget");
            dialog->show();

            connect(dialog, &WidgetDialogGenerator::widgetReady, this, &MainWindow::newWidget);
        });
    }
}

/* Slots */
void MainWindow::newWidget(BaseWidget *widget, WidgetData data) {
    widget->installEventFilter(this);
    data.tabIdx = m_centralWidget->currentIndex();
    m_widgets.insert(widget, data);
    relay();
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

        m_tabWidgets.append(tab);
        m_centralWidget->addTab(tab, tabName);
        m_centralWidget->setCurrentWidget(tab);
    }
}

void MainWindow::closeTab() {
    if (m_tabWidgets.empty()) return;
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
                widget->deleteLater();
            } else if (data.tabIdx > index) {
                data.tabIdx -= 1;

                m_widgets.remove(widget);
                m_widgets.insert(widget, data);
            }
        }
    }
    relay();
}

void MainWindow::renameTab() {
    if (m_tabWidgets.empty()) return;

    bool ok;
    QString tabName = QInputDialog::getText(this, "Tab Name", "Input new tab name", QLineEdit::Normal, m_centralWidget->tabText(m_centralWidget->currentIndex()), &ok);

    if (!tabName.isEmpty() && ok) {
        m_centralWidget->setTabText(m_centralWidget->currentIndex(), tabName);
    }
}

void MainWindow::setMaxSize() {
    if (m_tabWidgets.empty()) return;

    TabWidget *tab = m_tabWidgets.at(m_centralWidget->currentIndex());
    TabMaxSizeDialog *dialog = new TabMaxSizeDialog(this, tab->maxSize());
    dialog->show();

    connect(dialog, &TabMaxSizeDialog::dataReady, this, [this, tab](QPoint point) {
        tab->setMaxSize(point);
    });
}

// New Widget
void MainWindow::newWidgetPopup() {

    if (m_tabWidgets.length() == 0) {
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

        QObject *receiver = new QObject(this);
        connect(listDialog, &NewWidgetTreeDialog::widgetReady, receiver, [this, receiver](BaseWidget *widget, WidgetData data) {
            receiver->deleteLater();
            m_draggedWidget = widget;
            data.tabIdx = m_centralWidget->currentIndex();
            m_draggedWidgetData = data;

            dragStart(QCursor::pos(), QPoint(0, 0));

            // ensure connection only occurs once
            QObject *receiver2 = new QObject(this);
            connect(this, &MainWindow::dragDone, receiver2, [this, receiver2, widget](BaseWidget *, WidgetData data) {
                receiver2->deleteLater();

                WidgetDialogGenerator *dialog = new WidgetDialogGenerator(widget, this, false, data);
                dialog->setWindowTitle("New Widget");
                dialog->show();

                connect(dialog, &WidgetDialogGenerator::widgetReady, this, &MainWindow::newWidget);
            });
        });
        listDialog->show();
    }
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
