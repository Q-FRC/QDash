#ifndef NEWCAMERAVIEWDIALOG_H
#define NEWCAMERAVIEWDIALOG_H

#include "dialogs/NewWidgetDialog.h"

class NewCameraViewDialog : public NewWidgetDialog
{
protected:
    QLineEdit *m_urlEdit;
public:
    NewCameraViewDialog();
    virtual ~NewCameraViewDialog();
};

#endif // NEWCAMERAVIEWDIALOG_H
