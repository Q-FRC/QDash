#ifndef NEWWIDGETDIALOG_H
#define NEWWIDGETDIALOG_H
#include "networktables/NetworkTableEntry.h"

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
private:
    QFormLayout *m_layout;
    nt::NetworkTableEntry m_entry;

    bool m_isBooleanDisplay;

    QLabel *m_nameLabel;
    QLineEdit *m_nameInput;

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
    NewWidgetDialog(std::string ntTopic);
    virtual ~NewWidgetDialog();

signals:
    void dataReady(std::string topic, nt::NetworkTableType type, QString name, QColor trueColor, QColor falseColor, QList<int> widgetData);
};

#endif // NEWWIDGETDIALOG_H
