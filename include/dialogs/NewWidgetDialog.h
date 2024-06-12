#ifndef NEWWIDGETDIALOG_H
#define NEWWIDGETDIALOG_H
#include <ntcore.h>

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

    bool m_isBooleanDisplay;

    QLabel *m_nameLabel;
    QLineEdit *m_nameInput;

    QLabel *m_trueColorLabel;
    QWidget *m_trueColorInput;
    QHBoxLayout *m_trueColorInputLayout;
    QLineEdit *m_trueColorEdit;
    QPushButton *m_trueColorSelect;
    QAction *m_trueColorAction;

    QLabel *m_falseColorLabel;
    QWidget *m_falseColorInput;
    QHBoxLayout *m_falseColorInputLayout;
    QLineEdit *m_falseColorEdit;
    QPushButton *m_falseColorSelect;
    QAction *m_falseColorAction;

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
    void dataReady(std::string topic, NT_Type type, QString name, QColor trueColor, QColor falseColor, QList<int> widgetData);
};

#endif // NEWWIDGETDIALOG_H
