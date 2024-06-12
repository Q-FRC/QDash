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

#include "dialogs/NewWidgetDialog.h"

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
    QMap<BaseWidget *, QList<int>> m_widgets;

    bool m_needsRelay = true;

    void mousePressEvent(QMouseEvent *event);
public:
    MainWindow();
    virtual ~MainWindow();
    void update();

    void setNeedsRelay(bool needsRelay);
    QList<int> getWidgetData(BaseWidget *widget);

    void showNewWidgetDialog(NewWidgetDialog::WidgetTypes widgetType, std::string topic);

public slots:
    void newWidget(BaseWidget *widget, QList<int> data);

    void constructNewWidgetMenu(QMenu *menu);
};
