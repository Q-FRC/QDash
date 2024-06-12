#ifndef NEWSTRINGCHOOSERDIALOG_H
#define NEWSTRINGCHOOSERDIALOG_H

#include "dialogs/NewWidgetDialog.h"

class NewStringChooserDialog : public NewWidgetDialog
{
public:
    NewStringChooserDialog(std::string ntTopic, QWidget *parent, WidgetData data);
    virtual ~NewStringChooserDialog();
};
#endif // NEWSTRINGCHOOSERDIALOG_H
