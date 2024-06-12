// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "PitChecklist.h"

QStringList mechanicalCheckList = QStringList({
    "Swerve Bolts",
    "Arm Bolts",
    "Wrist Bolts",
    "Infeed Bolts",
    "Belly Pan",
    "Arm Bearings"
});

QStringList electricalCheckList = QStringList({
    "CAN Wires",
    "Wrist Encoder",
    "Power Wires",
    "Main Power",
    "PDH Ports",
    "Radio && Switch Wires"
});

QStringList controlsCheckList = QStringList({
    "Push Code",
    "Smoke Test"
});

PitChecklist::PitChecklist() {
    m_layout = new QGridLayout(this);

    // MECHANICAL
    m_mechanicalLayout = new QFormLayout(m_mechanicalWidget = new QWidget(this));
    m_mechanicalBoxes = QList<QCheckBox *>(mechanicalCheckList.size());

    m_mechanicalLabel = new QLabel("MECHANICAL", m_mechanicalWidget);

    QFont mechanicalFont = m_mechanicalLabel->font();
    mechanicalFont.setPixelSize(40);
    m_mechanicalLabel->setFont(mechanicalFont);

    m_mechanicalLayout->addRow(m_mechanicalLabel);

    for (qsizetype i = 0; i < mechanicalCheckList.size(); ++i) {
        QCheckBox *box = new QCheckBox(mechanicalCheckList[i], m_mechanicalWidget);

        box->setCheckable(true);

        m_mechanicalBoxes.append(box);
        m_mechanicalLayout->addRow(box);
    }

    m_layout->addWidget(m_mechanicalWidget, 0, 0, Qt::AlignmentFlag::AlignTop);

    // ELECTRICAL
    m_electricalLayout = new QFormLayout(m_electricalWidget = new QWidget(this));
    m_electricalBoxes = QList<QCheckBox *>(electricalCheckList.size());

    m_electricalLabel = new QLabel("ELECTRICAL", m_electricalWidget);

    QFont electricalFont = m_electricalLabel->font();
    electricalFont.setPixelSize(40);
    m_electricalLabel->setFont(electricalFont);

    m_electricalLayout->addRow(m_electricalLabel);

    for (qsizetype i = 0; i < electricalCheckList.size(); ++i) {
        QCheckBox *box = new QCheckBox(electricalCheckList[i], m_electricalWidget);
        box->setCheckable(true);

        m_electricalBoxes.append(box);
        m_electricalLayout->addRow(box);
    }

    m_layout->addWidget(m_electricalWidget, 0, 1, Qt::AlignmentFlag::AlignTop);

    // CONTROLS
    m_controlsLayout = new QFormLayout(m_controlsWidget = new QWidget(this));
    m_controlsBoxes = QList<QCheckBox *>(controlsCheckList.size());

    m_controlsLabel = new QLabel("CONTROLS", m_controlsWidget);

    QFont controlsFont = m_controlsLabel->font();
    controlsFont.setPixelSize(40);
    m_controlsLabel->setFont(controlsFont);

    m_controlsLayout->addRow(m_controlsLabel);

    for (qsizetype i = 0; i < controlsCheckList.size(); ++i) {
        QCheckBox *box = new QCheckBox(controlsCheckList[i], m_controlsWidget);
        box->setCheckable(true);

        m_controlsBoxes.append(box);
        m_controlsLayout->addRow(box);
    }

    m_layout->addWidget(m_controlsWidget, 1, 0, Qt::AlignmentFlag::AlignTop);


    // BUTTON :)
    m_clearBoxes = new QPushButton("Clear Checklist", this);
    m_layout->addWidget(m_clearBoxes, 1, 1, Qt::AlignmentFlag::AlignTop);

    m_clearAction = new QAction(m_clearBoxes);
    
    connect(m_clearAction, &QAction::triggered, this, [this] {
        for (QCheckBox *box : m_mechanicalBoxes) {
            box->setChecked(false);
        }
        for (QCheckBox *box : m_electricalBoxes) {
            box->setChecked(false);
        }
        for (QCheckBox *box : m_controlsBoxes) {
            box->setChecked(false);
        }
    });

}

PitChecklist::~PitChecklist() {}