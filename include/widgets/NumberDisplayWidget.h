#pragma once

#include "TextWidget.h"

class NumberDisplayWidget : public TextWidget
{
protected:
    double m_value;

    void keyPressEvent(QKeyEvent *event) override;
public:
    NumberDisplayWidget(const QString &title, const double &defaultValue, const QString &topic);
    virtual ~NumberDisplayWidget();

    QJsonObject saveObject() override;

    void update() override;
};
