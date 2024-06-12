#pragma once

#include "widgets/BaseWidget.h"
#include "widgets/TabWidget.h"

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QIcon>
#include <QPixmap>
#include <QStackedLayout>
#include <QMap>
#include <QMouseEvent>
#include <QTabWidget>
#include <QListWidget>
#include <QJsonDocument>

#include "Globals.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTabWidget *m_centralWidget;
    QStackedLayout *m_layout;

    QToolBar *m_toolbar;
    QMenuBar *m_menubar;

    QList<TabWidget *> m_tabWidgets;

    /**
     * Widget map. Int array format is as follows:
     * - tab idx
     * - row idx
     * - column idx
     * - rowspan
     * - colspan
    */
    QMap<BaseWidget *, WidgetData> m_widgets;

    bool m_needsRelay = true;

    QString m_filename{};

    void mousePressEvent(QMouseEvent *event);

    QMap<BaseWidget *, WidgetData> widgetsForTab(int tabIdx);    

    void setNeedsRelay(bool needsRelay);
    WidgetData getWidgetData(BaseWidget *widget);
public:
    MainWindow();
    virtual ~MainWindow();
    void update();

    // File I/O
    QJsonDocument saveObject();
    void loadObject(const QJsonDocument &doc);

public slots:
    void newWidget(BaseWidget *widget, WidgetData data);

    // NT Settings
    void ntSettings();

    // File Actions
    void save();
    void saveAs();
    void open();

    // Tab Actions
    void newTab();
    void closeTab();

    // New Widget
    void newWidgetPopup();

    // Camera View
    void newCameraView();
};
