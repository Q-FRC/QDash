#pragma once

#include "TextWidget.h"

class BaseWidget;

class IntegerDisplayWidget : public TextWidget
{
    Q_OBJECT

    Q_PROPERTY(int value MEMBER m_value)
protected:
    int m_value = 0;

    void keyPressEvent(QKeyEvent *event) override;
public:
    IntegerDisplayWidget(const QString &topic = "", const int &defaultValue = 0, const QString &title = "", const bool &ready = true);
    ~IntegerDisplayWidget();

    void setValue(const nt::Value &value) override;
};
