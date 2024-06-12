#ifndef NEWBOOLEANCHECKBOXDIALOG_H
#define NEWBOOLEANCHECKBOXDIALOG_H

#include "dialogs/NewWidgetDialog.h"

class NewBooleanCheckboxDialog : public NewWidgetDialog
{
public:
    NewBooleanCheckboxDialog(std::string ntTopic, QWidget *parent, WidgetData data);
    virtual ~NewBooleanCheckboxDialog();
};

#endif // NEWBOOLEANCHECKBOXDIALOG_H
