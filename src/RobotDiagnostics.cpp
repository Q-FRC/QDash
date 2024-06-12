#include "RobotDiagnostics.h"

QLabel *RobotDiagnostics::titleLabel(const QString &text)
{
    QLabel *label = new QLabel(text, this);

    QFont font = QFont(label->font());
    font.setBold(true);
    font.setItalic(true);

    label->setFont(font);

    return label;
}

QLabel *RobotDiagnostics::dataLabel(const QString &defaultText)
{
    QLabel *label = new QLabel(defaultText, this);
    label->setStyleSheet("background-color: black; color: white;");

    QFont font = QFont(label->font());
    font.setBold(true);

    label->setFont(font);

    return label;
}

RobotDiagnostics::RobotDiagnostics(const NT_Inst &inst)
{
    setStyleSheet("border: 2px solid purple;");

    m_layout = new QGridLayout(this);

    // UPPER ARM
    m_upperArmTitle = titleLabel("Upper Arm");
    m_upperArmData = dataLabel("3.0");
    m_upperArmSub = nt::Subscribe(nt::GetTopic(inst, "/SmartDashboard/Up Arm Pos"), NT_DOUBLE, "double");

    m_layout->addWidget(m_upperArmTitle, 0, 0);
    m_layout->addWidget(m_upperArmData, 1, 0);

    // LOWER ARM
    m_lowerArmTitle = titleLabel("Lower Arm");
    m_lowerArmData = dataLabel("3.0");
    m_lowerArmSub = nt::Subscribe(nt::GetTopic(inst, "/SmartDashboard/Low Arm Pos"), NT_DOUBLE, "double");

    m_layout->addWidget(m_lowerArmTitle, 0, 1);
    m_layout->addWidget(m_lowerArmData, 1, 1);

    // WRIST
    m_wristTitle = titleLabel("Wrist");
    m_wristData = dataLabel("275.0");
    m_wristSub = nt::Subscribe(nt::GetTopic(inst, "/SmartDashboard/Wrist Pos"), NT_DOUBLE, "double");

    m_layout->addWidget(m_wristTitle, 0, 2);
    m_layout->addWidget(m_wristData, 1, 2);

    // GAME PIECE MODE
    m_modeTitle = titleLabel("Mode");
    m_modeData = new QWidget(this);

    m_modeTrue = "purple";
    m_modeFalse = "orange";

    m_modeSub = nt::Subscribe(nt::GetTopic(inst, "/SmartDashboard/Mode"), NT_BOOLEAN, "boolean");

    m_layout->addWidget(m_modeTitle, 2, 0);
    m_layout->addWidget(m_modeData, 3, 0);
}

void RobotDiagnostics::updateData()
{
    m_upperArmData->setText(QString::number(nt::GetDouble(m_upperArmSub, 0.0)));
    m_lowerArmData->setText(QString::number(nt::GetDouble(m_lowerArmSub, 0.0)));
    m_wristData->setText(QString::number(nt::GetDouble(m_wristSub, 0.0)));

    bool mode = nt::GetBoolean(m_modeSub, false);
    m_modeData->setStyleSheet("background-color: " + (mode ? m_modeTrue : m_modeFalse) + ";");
}

RobotDiagnostics::~RobotDiagnostics() {}