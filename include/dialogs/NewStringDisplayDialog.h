#ifndef NEWSTRINGDISPLAYDIALOG_H
#define NEWSTRINGDISPLAYDIALOG_H

#include "dialogs/NewWidgetDialog.h"

class NewStringDisplayDialog : public NewWidgetDialog
{
public:
    NewStringDisplayDialog(std::string ntTopic);
    virtual ~NewStringDisplayDialog();
};

#endif // NEWSTRINGDISPLAYDIALOG_H
