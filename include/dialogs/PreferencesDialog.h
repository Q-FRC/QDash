#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>

extern QMap<QString, QString> StyleSheetMap;

class PreferencesDialog : public QDialog
{
    Q_OBJECT
private:
    QFormLayout *m_layout;
    QComboBox *m_choices;
    QDialogButtonBox *m_buttonBox;
public:
    PreferencesDialog(QWidget *parent);
signals:
    void styleSheetSet(QString styleSheet);
};

#endif // PREFERENCESDIALOG_H
