#ifndef NEWWIDGETDIALOG_H
#define NEWWIDGETDIALOG_H
#include "networktables/NetworkTableEntry.h"

#include "Globals.h"
#include "widgets/BaseWidget.h"

#include <QDialog>
#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>

class NewWidgetDialog : public QDialog
{
        Q_OBJECT
protected:
    QFormLayout *m_layout;
    nt::NetworkTableEntry m_entry;

    QLabel *m_nameLabel;
    QLineEdit *m_nameInput;

    QLabel *m_rowLabel;
    QSpinBox *m_rowInput;

    QLabel *m_columnLabel;
    QSpinBox *m_columnInput;

    QLabel *m_rowSpanLabel;
    QSpinBox *m_rowSpanInput;

    QLabel *m_columnSpanLabel;
    QSpinBox *m_columnSpanInput;

    QDialogButtonBox *m_buttonBox;
public:
    NewWidgetDialog(std::string ntTopic, QWidget *parent, const WidgetData &data);
    virtual ~NewWidgetDialog();

    static NewWidgetDialog *fromWidgetType(WidgetTypes type, std::string ntTopic, QWidget *parent = nullptr, const WidgetData &data = makeWidgetData(0, 0, 0, 0, 0));

signals:
    void widgetReady(BaseWidget *widget, WidgetData data);
};

#endif // NEWWIDGETDIALOG_H
