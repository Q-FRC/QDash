#pragma once

// #include "CentralWidget.h"

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

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTabWidget *m_centralWidget;

    QToolBar *m_toolbar;
    QMenuBar *m_menubar;

    QList<TabWidget *> m_tabWidgets;

    QMap<BaseWidget *, WidgetData> m_widgets;

    QString m_filename{};

    // Dragging
    QPoint m_dragStart;
    QPoint m_dragOffset;
    BaseWidget *m_draggedWidget;
    WidgetData m_draggedWidgetData;
    bool m_dragging = false;

    // Resizing
    ResizeDirection m_currentResize;
    QRect m_initialSize;
    bool m_resizing = false;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

    void dragStart(QPoint point, QPoint offset);
    void dragMove(QPoint point);
    void dragRelease(QPoint point);

    void resizeStart(QPoint point);
    void resizeMove(QPoint point);
    void resizeRelease(QPoint point);

    QMap<BaseWidget *, WidgetData> widgetsForTab(int tabIdx);

    void relay();

    void makeNewWidget(WidgetTypes type);
public:
    MainWindow();
    virtual ~MainWindow();

    void update();

    // File I/O
    QJsonDocument saveObject();
    void loadObject(const QJsonDocument &doc);

    bool positionContainsWidget(WidgetData data);
    bool eventFilter(QObject *object, QEvent *event);

public slots:
    void newWidget(BaseWidget *widget, WidgetData data);
    void deleteWidget(BaseWidget *widget);

    // NT Settings
    void ntSettingsPopup();
    void setNtSettings(ServerData data);

    // File Actions
    void save();
    void saveAs();
    void openDialog();
    void open(QFile &file);

    // Tab Actions
    void newTab();
    void closeTab();
    void setMaxSize();
    void renameTab();

    // New Widget
    void newWidgetPopup();
    void configNewWidget(BaseWidget *widget, WidgetData data);
    void beginNewWidgetDrag(BaseWidget *widget, WidgetData data);

    // About Menu
    void aboutDialog();
signals:
    void dragDone(BaseWidget *widget, WidgetData data);
};
