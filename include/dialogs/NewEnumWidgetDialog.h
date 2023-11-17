#ifndef NEWENUMWIDGETDIALOG_H
#define NEWENUMWIDGETDIALOG_H

#include "dialogs/NewWidgetDialog.h"

#include <QTableWidget>

class NewEnumWidgetDialog : public NewWidgetDialog
{
private:
    QTableWidget *m_tableWidget;

    QPushButton *m_addItemButton;
    QPushButton *m_removeItemButton;
    QPushButton *m_colorButton;

    QHBoxLayout *m_buttonLayout;
public:
    NewEnumWidgetDialog(std::string ntTopic, QWidget *parent, WidgetData data);
    virtual ~NewEnumWidgetDialog();

    QMap<QString, QColor> serializeTable();
};

#endif // NEWENUMWIDGETDIALOG_H
