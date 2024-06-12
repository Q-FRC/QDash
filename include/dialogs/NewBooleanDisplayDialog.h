#ifndef NEWBOOLEANDISPLAYDIALOG_H
#define NEWBOOLEANDISPLAYDIALOG_H

#include "dialogs/NewWidgetDialog.h"

class NewBooleanDisplayDialog : public NewWidgetDialog
{
private:
    QLabel *m_trueColorLabel;
    QWidget *m_trueColorInput;
    QHBoxLayout *m_trueColorInputLayout;
    QLineEdit *m_trueColorEdit;
    QPushButton *m_trueColorSelect;

    QLabel *m_falseColorLabel;
    QWidget *m_falseColorInput;
    QHBoxLayout *m_falseColorInputLayout;
    QLineEdit *m_falseColorEdit;
    QPushButton *m_falseColorSelect;
public:
    NewBooleanDisplayDialog(std::string ntTopic);
    virtual ~NewBooleanDisplayDialog();
};

#endif // NEWBOOLEANDISPLAYDIALOG_H
