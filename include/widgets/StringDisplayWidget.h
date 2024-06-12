#pragma once

#include "TextWidget.h"

class StringDisplayWidget : public TextWidget
{
protected:
    QString m_value;

    void keyPressEvent(QKeyEvent *event) override;
public:
    StringDisplayWidget(const QString &title, const QString &defaultValue, const QString &topic);
    virtual ~StringDisplayWidget();

    void update();
};
