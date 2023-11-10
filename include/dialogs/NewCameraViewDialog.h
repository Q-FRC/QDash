#ifndef NEWCAMERAVIEWDIALOG_H
#define NEWCAMERAVIEWDIALOG_H

#include "dialogs/NewWidgetDialog.h"

class NewCameraViewDialog : public NewWidgetDialog
{
protected:
    QLineEdit *m_urlEdit;
public:
    // default constructor errors are really funny
    NewCameraViewDialog(QWidget *parent, WidgetData data = WidgetData{0, 0, 0, 0, 0});
    virtual ~NewCameraViewDialog();
};

#endif // NEWCAMERAVIEWDIALOG_H
