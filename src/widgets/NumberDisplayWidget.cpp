// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "widgets/NumberDisplayWidget.h"

NumberDisplayWidget::NumberDisplayWidget(const QString &title, const double &defaultValue, const QString &topic) : TextWidget(title, QString::number(defaultValue), topic, NT_DOUBLE)
{
    m_value = defaultValue;
}

NumberDisplayWidget::~NumberDisplayWidget() {}

void NumberDisplayWidget::update() {
    double value = nt::GetDouble(m_entry, m_value);

    m_value = value;
    setText(QString::number(value));
}
