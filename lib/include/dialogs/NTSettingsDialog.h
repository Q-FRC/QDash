#ifndef NTSETTINGSDIALOG_H
#define NTSETTINGSDIALOG_H

#include "Globals.h"
#include <QDialog>

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>

#include "ui_NTSettingsDialog.h"

class NTSettingsDialog : public QDialog, Ui::NTSettingsDialog
{
    Q_OBJECT
public:
    NTSettingsDialog(QWidget *parent = nullptr);
    virtual ~NTSettingsDialog();

public slots:
    void serializeData();
signals:
    void dataReady(ServerData data);
};

#endif // NTSETTINGSDIALOG_H
