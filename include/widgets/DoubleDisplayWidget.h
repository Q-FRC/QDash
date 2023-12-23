#pragma once

#include "TextWidget.h"

class BaseWidget;

class DoubleDisplayWidget : public TextWidget
{
    Q_OBJECT

    Q_PROPERTY(double value MEMBER m_value)
protected:
    double m_value = 0.;

    QHash<Globals::Topic, QColor> m_map{};

    void keyPressEvent(QKeyEvent *event) override;
public:
    DoubleDisplayWidget(const QString &topic = "", const double &defaultValue = 0., const QString &title = "", const bool &ready = true);
    ~DoubleDisplayWidget();

    void setValue(const nt::Value &value) override;
};
