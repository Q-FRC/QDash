#ifndef SENDABLEFIELDWIDGET_H
#define SENDABLEFIELDWIDGET_H

#include "widgets/FieldWidget.h"

class SendableFieldWidget : public FieldWidget
{
public:
    SendableFieldWidget(QString topic = "") : FieldWidget(topic + "/Robot") {}
    ~SendableFieldWidget();

    inline static WidgetTypes WidgetType = WidgetTypes::SendableField;
    inline static TopicTypes TopicType = TopicTypes::Field2d;
    inline static QString SendableName = "Field2d";
    inline static QString DisplayName = "Field2d";
};

#endif // SENDABLEFIELDWIDGET_H
