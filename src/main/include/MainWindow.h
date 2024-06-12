// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "ntcore.h"

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QIcon>
#include <QPixmap>
#include <QStackedLayout>

class RobotDiagnostics;
class PitChecklist;
class EventData;

class MainWindow : public QMainWindow
{
private:
    QWidget *m_centralWidget;
    QStackedLayout *m_layout;

    QToolBar *m_toolbar;

    QAction *m_diagnosticsAction;
    QAction *m_checklistAction;
    QAction *m_eventDataAction;

    RobotDiagnostics *m_diagnosticsWidget;
    PitChecklist *m_checklistWidget;
    EventData *m_eventDataWidget;
    
public:
    MainWindow(RobotDiagnostics *diagnostics, PitChecklist *checklist, EventData *eventData);
    virtual ~MainWindow();
};
