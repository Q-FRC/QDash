// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "ntcore.h"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

class RobotDiagnostics : public QWidget
{
private:
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

    QLabel *titleLabel(const QString &text);
    QLabel *dataLabel(const QString &defaultText);
public:
    RobotDiagnostics(const NT_Inst &inst);
    virtual ~RobotDiagnostics();

    void updateData();
};
