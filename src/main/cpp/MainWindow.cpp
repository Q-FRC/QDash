// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "MainWindow.h"

QLabel *titleLabel(QString text, QWidget *parent)
{
    QLabel *label = new QLabel(text, parent);

    QFont font = QFont(label->font());
    font.setBold(true);
    font.setItalic(true);

    label->setFont(font);

    return label;
}

QLabel *dataLabel(QString defaultText, QWidget *parent)
{
    QLabel *label = new QLabel(defaultText, parent);
    label->setStyleSheet("background-color: black; color: white;");

    QFont font = QFont(label->font());
    font.setBold(true);

    label->setFont(font);

    return label;
}

MainWindow::MainWindow(NT_Inst inst)
{
    setCentralWidget(m_centralWidget = new QWidget);
    m_centralWidget->setStyleSheet("border: 2px solid purple;");

    m_layout = new QGridLayout;

    // UPPER ARM
    m_upperArmTitle = titleLabel("Upper Arm", m_centralWidget);
    m_upperArmData = dataLabel("3.0", m_centralWidget);
    m_upperArmSub = nt::Subscribe(nt::GetTopic(inst, "/SmartDashboard/Upper Arm"), NT_DOUBLE, "double");

    m_layout->addWidget(m_upperArmTitle, 0, 0);
    m_layout->addWidget(m_upperArmData, 1, 0);

    // LOWER ARM
    m_lowerArmTitle = titleLabel("Lower Arm", m_centralWidget);
    m_lowerArmData = dataLabel("3.0", m_centralWidget);
    m_lowerArmSub = nt::Subscribe(nt::GetTopic(inst, "/SmartDashboard/Lower Arm"), NT_DOUBLE, "double");

    m_layout->addWidget(m_lowerArmTitle, 0, 1);
    m_layout->addWidget(m_lowerArmData, 1, 1);

    // WRIST
    m_wristTitle = titleLabel("Wrist", m_centralWidget);
    m_wristData = dataLabel("275.0", m_centralWidget);
    m_wristSub = nt::Subscribe(nt::GetTopic(inst, "/SmartDashboard/Wrist"), NT_DOUBLE, "double");

    m_layout->addWidget(m_wristTitle, 0, 2);
    m_layout->addWidget(m_wristData, 1, 2);

    // GAME PIECE MODE
    m_modeTitle = titleLabel("Mode", m_centralWidget);
    m_modeData = new QWidget(m_centralWidget);

    m_modeTrue = "purple";
    m_modeFalse = "orange";

    m_modeSub = nt::Subscribe(nt::GetTopic(inst, "/SmartDashboard/Mode"), NT_BOOLEAN, "boolean");

    m_layout->addWidget(m_modeTitle, 2, 0);
    m_layout->addWidget(m_modeData, 3, 0);

    // END WIDGETS
    m_centralWidget->setLayout(m_layout);
}

void MainWindow::updateLabels()
{
    m_upperArmData->setText(QString::number(nt::GetDouble(m_upperArmSub, 0.0)));
    m_lowerArmData->setText(QString::number(nt::GetDouble(m_lowerArmSub, 0.0)));
    m_wristData->setText(QString::number(nt::GetDouble(m_wristSub, 0.0)));

    bool mode = nt::GetBoolean(m_modeSub, false);
    m_modeData->setStyleSheet("background-color: " + (mode ? m_modeTrue : m_modeFalse) + ";");
}

MainWindow::~MainWindow() {}