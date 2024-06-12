// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include "TextWidget.h"

#include <QFrame>

class BooleanDisplayWidget : public BaseWidget
{
protected:
    bool m_value;

    QFrame *m_colorWidget;

    QColor m_trueColor = Qt::green;
    QColor m_falseColor = Qt::red;
public:
    BooleanDisplayWidget(const QString &title, const bool &defaultValue, const QString &topic);
    virtual ~BooleanDisplayWidget();

    QColor trueColor();
    void setTrueColor(const QColor &color);

    QColor falseColor();
    void setFalseColor(const QColor &color);

    void update();
};
