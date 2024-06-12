// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <QWidget>
#include <QCheckBox>
#include <QGridLayout>
#include <QList>
#include <QPushButton>
#include <QAction>
#include <QLabel>
#include <QFormLayout>

class PitChecklist : public QWidget
{
private:
    QLabel *m_mechanicalLabel;
    QList<QCheckBox *> m_mechanicalBoxes;
    QFormLayout *m_mechanicalLayout;
    QWidget *m_mechanicalWidget;

    QLabel *m_electricalLabel;
    QList<QCheckBox *> m_electricalBoxes;
    QFormLayout *m_electricalLayout;
    QWidget *m_electricalWidget;

    QLabel *m_controlsLabel;
    QList<QCheckBox *> m_controlsBoxes;
    QFormLayout *m_controlsLayout;
    QWidget *m_controlsWidget;

    QGridLayout *m_layout;

    QPushButton *m_clearBoxes;
    QAction *m_clearAction;
public:
    PitChecklist();
    virtual ~PitChecklist();
};
