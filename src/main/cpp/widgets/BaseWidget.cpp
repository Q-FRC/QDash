// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "widgets/BaseWidget.h"

BaseWidget::BaseWidget(const QString &title, const QString &topic, const NT_Type &type)
{
    m_layout = new QGridLayout(this);
    m_title = new QLabel(title, this);
    m_entry = nt::GetEntry(nt::GetDefaultInstance(), topic.toStdString());
    m_type = type;

    setStyleSheet("background-color: white; border: 2px solid yellow; color: black;");
}

QFont BaseWidget::titleFont()
{
    return m_title->font();
}

void BaseWidget::setTitleFont(const QFont &font)
{
    m_title->setFont(font);
}

BaseWidget::~BaseWidget() {}

QString BaseWidget::title()
{
    return m_title->text();
}

void BaseWidget::setTitle(const QString &title)
{
    m_title->setText(title);
}

std::string BaseWidget::topic()
{
    return nt::GetTopicName(nt::GetTopicFromHandle(m_entry));
}

void BaseWidget::setTopic(const std::string &topic)
{
    m_entry = nt::GetEntry(nt::GetDefaultInstance(), topic);
}

NT_Type BaseWidget::type()
{
    return m_type;
}

void BaseWidget::setType(const NT_Type &type)
{
    m_type = type;
}