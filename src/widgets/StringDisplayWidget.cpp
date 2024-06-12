// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "widgets/StringDisplayWidget.h"

StringDisplayWidget::StringDisplayWidget(const QString &title, const QString &defaultValue, const QString &topic) : TextWidget(title, defaultValue, topic, NT_STRING)
{
    m_value = defaultValue;
}

StringDisplayWidget::~StringDisplayWidget() {}

void StringDisplayWidget::update() {
    QString value = QString::fromStdString(nt::GetString(m_entry, m_value.toStdString()));

    m_value = value;
    setText(value);
}