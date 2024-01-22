#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>

#include "ui_PreferencesDialog.h"

extern QMap<QString, QString> StyleSheetMap;

class PreferencesDialog : public QDialog, Ui::PreferencesDialog
{
    Q_OBJECT

public:
    PreferencesDialog(QWidget *parent);
public slots:
    void emitSheet();
signals:
    void styleSheetSet(QString styleSheet);
};

#endif // PREFERENCESDIALOG_H
