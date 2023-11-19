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
    StringDisplayWidget(const QString &title, const QString &defaultValue, const QString &topic);
    ~StringDisplayWidget();

    QJsonObject saveObject() override;

    void update();
};
