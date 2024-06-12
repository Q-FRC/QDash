// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "MainWindow.h"

#include <QToolBar>

MainWindow::MainWindow()
{
    setCentralWidget(m_centralWidget = new QWidget);

    // Use a stacked layout to have multiple widgets available to switch between
    // Using setCentralWidget is not possible, because upon switching the central widget
    // the previously set widget is destroyed.
    m_layout = new QStackedLayout(m_centralWidget);

    m_coolWidget = new QWidget(this);
    m_coolLayout = new QGridLayout(m_coolWidget);

    NumberDisplayWidget *number = new NumberDisplayWidget("Test", 25.5, "deez");

    m_coolLayout->addWidget(number, 0, 0);

    m_layout->addWidget(m_coolWidget);

    m_widgets.push_back(number);

    m_toolbar = new QToolBar(this);

    //     // DIAGNOSTICS WIDGET
    //     m_diagnosticsWidget = diagnostics;
    //     m_layout->addWidget(m_diagnosticsWidget);

    //     m_diagnosticsAction = new QAction("Robot Diagnostics", m_toolbar);
    //     connect(m_diagnosticsAction, &QAction::triggered, [this]
    //             { m_layout->setCurrentWidget(m_diagnosticsWidget); });
    //     m_toolbar->addAction(m_diagnosticsAction);

    addToolBar(Qt::ToolBarArea::BottomToolBarArea, m_toolbar);
}

MainWindow::~MainWindow() {}

void MainWindow::update() {
    for (BaseWidget *w : m_widgets) {
        w->update();
    }
}