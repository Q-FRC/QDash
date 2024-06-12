// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "TextWidget.h"

class NumberDisplayWidget : public TextWidget
{
protected:
    double m_value;
public:
    NumberDisplayWidget(const QString &title, const double &defaultValue, const QString &topic);
    virtual ~NumberDisplayWidget();

    void update();
};
