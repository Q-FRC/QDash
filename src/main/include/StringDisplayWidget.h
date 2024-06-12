// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "TextWidget.h"

class StringDisplayWidget : public TextWidget
{
protected:
    QString m_value;
public:
    StringDisplayWidget(const QString &title, const QString &defaultValue, const QString &topic);
    virtual ~StringDisplayWidget();

    void update();
};
