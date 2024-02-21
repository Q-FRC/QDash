#pragma once

#include "TextWidget.h"

class StringDisplayWidget : public TextWidget
{
    Q_OBJECT

    Q_PROPERTY(QString value MEMBER m_value)
    Q_PROPERTY(QString Topic READ topic WRITE setTopic REQUIRED)
protected:
    QString m_value = "";

    void keyPressEvent(QKeyEvent *event) override;
public:
    StringDisplayWidget(const QString &topic = "", const QString &defaultValue = "", const QString &title = "");
    ~StringDisplayWidget();

    void setTopic(const QString &topic) override;
    void setValue(const nt::Value &value, QString label = "", bool force = false) override;

    inline static WidgetTypes WidgetType = WidgetTypes::StringDisplay;
    inline static TopicTypes TopicType = TopicTypes::String;
    inline static QString SendableName = "";
    inline static QString DisplayName = "String Display";
};
