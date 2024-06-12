// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "BaseWidget.h"
#include "NumberDisplayWidget.h"
#include "StringDisplayWidget.h"
#include "BooleanDisplayWidget.h"

#include "ntcore.h"

#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QIcon>
#include <QPixmap>
#include <QStackedLayout>

class MainWindow : public QMainWindow
{
private:
    QWidget *m_centralWidget;
    QStackedLayout *m_layout;

    QToolBar *m_toolbar;

    QWidget *m_coolWidget;
    QGridLayout *m_coolLayout;

    QList<BaseWidget *> m_widgets;
public:
    MainWindow();
    virtual ~MainWindow();
    void update();
};
