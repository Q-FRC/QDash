#pragma once

#include "TextWidget.h"

class BaseWidget;

class NumberDisplayWidget : public TextWidget
{
    Q_OBJECT

    Q_PROPERTY(double value MEMBER m_value)
protected:
    double m_value = 0.;

    void keyPressEvent(QKeyEvent *event) override;
public:
    NumberDisplayWidget(const WidgetTypes &type, const QString &title, const double &defaultValue, const QString &topic);
    ~NumberDisplayWidget();

    QJsonObject saveObject() override;

    void update() override;
};
