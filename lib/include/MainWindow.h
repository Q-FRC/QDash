#pragma once

#include "widgets/BaseWidget.h"
#include "widgets/TabWidget.h"

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QMap>
#include <QMouseEvent>
#include <QJsonDocument>
#include <QTabWidget>

#include "Globals.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, Ui::MainWindow
{
    Q_OBJECT
private:
    QList<TabWidget *> m_tabs;

    QString m_filename{};
    int m_lastIdx = 0;

    void makeNewWidget(WidgetTypes type);
public:
    MainWindow();
    virtual ~MainWindow();

    TabWidget *currentTab();
    int currentTabIdx();

    TabWidget *tabNamed(QString name);
    void selectTab(TabWidget *tab);

    // File I/O
    QJsonDocument saveObject();
    void loadObject(const QJsonDocument &doc);

public slots:
    // Internal Stuff
    void forceUpdateTab(int idx);
    void moveTab(int from, int to);

    // Preferences
    void preferences();

    // NT Settings
    void ntSettingsPopup();
    void setNtSettings(ServerData data);

    // File Actions
    void save();
    void saveAs();
    void openDialog();
    void open(QFile &file);

    void refreshRecentFiles();
    void addRecentFile(QFile &file);

    // Tab Actions
    void newTab();
    void closeTab();
    void setMaxSize();
    void renameTab();

    // New Widget
    void newWidgetPopup();
    void beginNewWidgetDrag(BaseWidget *widget, WidgetData data);
    void newCameraView();
    void newGraph();
    void newSwerve();

    // Camera
    void cameraServerPopup();

    // About Menu
    void about();
    void aboutQt();

signals:
    void switchTopicChanged();
};
