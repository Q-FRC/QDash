#ifndef NEWDOUBLEDIALDIALOG_H
#define NEWDOUBLEDIALDIALOG_H

#include "dialogs/NewWidgetDialog.h"

#include <QDoubleSpinBox>

class NewDoubleDialDialog : public NewWidgetDialog
{
protected:
    QDoubleSpinBox *m_minInput;
    QDoubleSpinBox *m_maxInput;
public:
    NewDoubleDialDialog(std::string ntTopic);
    virtual ~NewDoubleDialDialog();
};

#endif // NEWDOUBLEDIALDIALOG_H
