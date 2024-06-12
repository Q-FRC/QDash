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

class MainWindow : public QMainWindow
{
private:
    QWidget *m_centralWidget;
    QWidget *m_menuWidget;

    QGridLayout *m_layout;

    QLabel *m_upperArmTitle;
    QLabel *m_upperArmData;
    NT_Subscriber m_upperArmSub;

    QLabel *m_lowerArmTitle;
    QLabel *m_lowerArmData;
    NT_Subscriber m_lowerArmSub;

    QLabel *m_wristTitle;
    QLabel *m_wristData;
    NT_Subscriber m_wristSub;

    QLabel *m_modeTitle;
    QWidget *m_modeData;
    QString m_modeTrue;
    QString m_modeFalse;
    NT_Subscriber m_modeSub;
    
public:
    MainWindow(NT_Inst inst);
    virtual ~MainWindow();

    void updateLabels();
};
