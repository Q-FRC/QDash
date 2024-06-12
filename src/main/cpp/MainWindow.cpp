// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "MainWindow.h"
#include "RobotDiagnostics.h"
#include "PitChecklist.h"
#include "EventData.h"

#include <QToolBar>

MainWindow::MainWindow(RobotDiagnostics *diagnostics, PitChecklist *checklist, EventData *eventData)
{
    setCentralWidget(m_centralWidget = new QWidget);

    // Use a stacked layout to have multiple widgets available to switch between
    // Using setCentralWidget is not possible, because upon switching the central widget
    // the previously set widget is destroyed.
    m_layout = new QStackedLayout(m_centralWidget);

    m_toolbar = new QToolBar(this);

    // DIAGNOSTICS WIDGET
    m_diagnosticsWidget = diagnostics;
    m_layout->addWidget(m_diagnosticsWidget);

    m_diagnosticsAction = new QAction("Robot Diagnostics", m_toolbar);
    connect(m_diagnosticsAction, &QAction::triggered, [this]
            { m_layout->setCurrentWidget(m_diagnosticsWidget); });
    m_toolbar->addAction(m_diagnosticsAction);

    // CHECKLIST
    m_checklistWidget = checklist;
    m_layout->addWidget(m_checklistWidget);

    m_checklistAction = new QAction("Pit Checklist", m_toolbar);
    connect(m_checklistAction, &QAction::triggered, [this]
            { m_layout->setCurrentWidget(m_checklistWidget); });
    m_toolbar->addAction(m_checklistAction);

    // EVENT DATA
    m_eventDataWidget = eventData;
    m_layout->addWidget(m_eventDataWidget);

    m_eventDataAction = new QAction("Event Data", m_toolbar);
    connect(m_eventDataAction, &QAction::triggered, [this]
            { m_layout->setCurrentWidget(m_eventDataWidget); });
    m_toolbar->addAction(m_eventDataAction);

    addToolBar(Qt::ToolBarArea::BottomToolBarArea, m_toolbar);
}

MainWindow::~MainWindow() {}