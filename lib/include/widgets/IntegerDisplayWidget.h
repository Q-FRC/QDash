#pragma once

#include "TextWidget.h"

class BaseWidget;

class IntegerDisplayWidget : public TextWidget
{
    Q_OBJECT

    Q_PROPERTY(int value MEMBER m_value)
    Q_PROPERTY(QString Topic READ topic WRITE setTopic REQUIRED)
protected:
    int m_value = 0;

    void keyPressEvent(QKeyEvent *event) override;
public:
    IntegerDisplayWidget(const QString &topic = "", const int &defaultValue = 0, const QString &title = "", const bool &ready = true);
    ~IntegerDisplayWidget();

    void setTopic(const QString &topic) override;
    void setValue(const nt::Value &value, QString label = "", bool force = false) override;

    inline static WidgetTypes WidgetType = WidgetTypes::IntegerDisplay;
    inline static TopicTypes TopicType = TopicTypes::Int;
    inline static QString SendableName = "";
    inline static QString DisplayName = "Int Display";
};
