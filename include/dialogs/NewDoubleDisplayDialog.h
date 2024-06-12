#ifndef NEWDOUBLEDISPLAYDIALOG_H
#define NEWDOUBLEDISPLAYDIALOG_H

#include "dialogs/NewWidgetDialog.h"

class NewDoubleDisplayDialog : public NewWidgetDialog
{
public:
    NewDoubleDisplayDialog(std::string ntTopic, QWidget *parent, WidgetData data);
    virtual ~NewDoubleDisplayDialog();
};

#endif // NEWDOUBLEDISPLAYDIALOG_H
