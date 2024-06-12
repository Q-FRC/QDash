// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "BooleanDisplayWidget.h"

BooleanDisplayWidget::BooleanDisplayWidget(const QString &title, const bool &defaultValue, const QString &topic) : BaseWidget(title, topic, NT_BOOLEAN)
{
    m_value = defaultValue;
    m_colorWidget = new QWidget(this);

    m_layout->addWidget(m_title, 0, 0, 1, 1, Qt::AlignHCenter | Qt::AlignCenter);
    m_layout->addWidget(m_colorWidget, 1, 0, 3, 0, Qt::AlignHCenter | Qt::AlignCenter);
}

BooleanDisplayWidget::~BooleanDisplayWidget() {}

QColor BooleanDisplayWidget::trueColor() {
    return m_trueColor;
}

void BooleanDisplayWidget::setTrueColor(const QColor &color) {
    m_trueColor = color;
}

QColor BooleanDisplayWidget::falseColor() {
    return m_falseColor;
}

void BooleanDisplayWidget::setFalseColor(const QColor &color) {
    m_falseColor = color;
}

void BooleanDisplayWidget::update() {
    bool value = nt::GetBoolean(m_entry, m_value);

    m_value = value;

    m_colorWidget->setStyleSheet("background-color: " + (value ? m_trueColor : m_falseColor).name() + ";");
}