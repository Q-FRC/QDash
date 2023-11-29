#ifndef SENDABLEFIELDWIDGET_H
#define SENDABLEFIELDWIDGET_H

#include "widgets/BaseWidget.h"

class SendableFieldWidget
{
public:
    static BaseWidget *fromJson(QJsonObject obj);
};

#endif // SENDABLEFIELDWIDGET_H
