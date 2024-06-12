#ifndef SENDABLEFIELDWIDGET_H
#define SENDABLEFIELDWIDGET_H

#include "widgets/FieldWidget.h"

class SendableFieldWidget : public FieldWidget
{
public:
    SendableFieldWidget(QString topic = "") : FieldWidget(topic + "/Robot") {}
    ~SendableFieldWidget();
};

#endif // SENDABLEFIELDWIDGET_H
