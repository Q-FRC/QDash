// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "ntcore.h"

#include <QWidget>
#include <QLabel>
#include <QGridLayout>

class BaseWidget : public QWidget
{
protected:
    QGridLayout *m_layout;

    QLabel *m_title;

    NT_Entry m_entry;
    NT_Type m_type;
public:
    BaseWidget(const QString &title, const QString &topic, const NT_Type &type);
    virtual ~BaseWidget();

    QFont titleFont();
    void setTitleFont(const QFont &font);

    QString title();
    void setTitle(const QString &title);

    std::string topic();
    void setTopic(const std::string &topic);

    NT_Type type();
    void setType(const NT_Type &type);

    virtual void update() {}
};
