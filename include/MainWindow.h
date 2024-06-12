// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "widgets/BaseWidget.h"
#include "widgets/NumberDisplayWidget.h"
#include "widgets/StringDisplayWidget.h"
#include "widgets/BooleanDisplayWidget.h"
#include "widgets/TabWidget.h"

#include "ntcore.h"

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QIcon>
#include <QPixmap>
#include <QStackedLayout>
#include <QMap>

class MainWindow : public QMainWindow
{
private:
    QWidget *m_centralWidget;
    QStackedLayout *m_layout;

    QToolBar *m_toolbar;
    QMenuBar *m_menubar;

    TabWidget *m_tabWidget;

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
public:
    MainWindow();
    virtual ~MainWindow();
    void update();

    void setNeedsRelay(bool needsRelay);
    QList<int> getWidgetData(BaseWidget *widget);
};
