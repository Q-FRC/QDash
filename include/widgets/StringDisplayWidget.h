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
