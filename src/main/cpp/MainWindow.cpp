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

    m_tabWidget = new TabWidget(QPoint(3, 3));

    BooleanDisplayWidget *boolean = new BooleanDisplayWidget("Test", "yoooo", "deez");
    boolean->setTrueColor(Qt::blue);
    boolean->setFalseColor(Qt::yellow);

    boolean->update();

    NumberDisplayWidget *number = new NumberDisplayWidget("Numero", 25.3, "numero");
    StringDisplayWidget *string = new StringDisplayWidget("String", "hola", "stringio");

    m_layout->addWidget(m_tabWidget);

    // FIXME: wacky
    int booleanData[] = {0, 0, 0, 1, 1};
    m_widgets.insert(boolean, booleanData);

    int numberData[] = {0, 0, 1, 2, 1};
    m_widgets.insert(number, numberData);

    int stringData[] = {0, 1, 0, 1, 1};
    m_widgets.insert(string, stringData);

    QMapIterator<BaseWidget *, int *> iterator(m_widgets);

    while (iterator.hasNext())
    {
        iterator.next();
        int *data = iterator.value();
        m_tabWidget->layout()->addWidget(iterator.key(), data[1], data[2], data[3], data[4]);
    }

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

void MainWindow::update()
{
    QMapIterator<BaseWidget *, int *> iterator(m_widgets);

    while (iterator.hasNext())
    {
        iterator.next();
        iterator.key()->update();
    }
}