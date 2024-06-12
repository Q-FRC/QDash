#pragma once

#include "TextWidget.h"

class StringDisplayWidget : public TextWidget
{
    Q_OBJECT

    Q_PROPERTY(QString value MEMBER m_value)
protected:
    QString m_value = "";

    void keyPressEvent(QKeyEvent *event) override;
public:
    StringDisplayWidget(const QString &topic = "", const QString &defaultValue = "", const QString &title = "");
    ~StringDisplayWidget();

    void setValue(const nt::Value &value) override;
};
