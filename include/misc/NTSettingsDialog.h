#ifndef NTSETTINGSDIALOG_H
#define NTSETTINGSDIALOG_H

#include "Globals.h"
#include <QDialog>

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>

class NTSettingsDialog : public QDialog
{
    Q_OBJECT
private:
    QFormLayout *m_layout;

    QLabel *m_addressLabel;
    QLineEdit *m_addressEdit;

    QLabel *m_numberLabel;
    QCheckBox *m_numberBox;

    QLabel *m_portLabel;
    QLineEdit *m_portEdit;

    QDialogButtonBox *m_buttonBox;

public:
    NTSettingsDialog(QWidget *parent = nullptr);
    virtual ~NTSettingsDialog();

signals:
    void dataReady(ServerData data);
};

#endif // NTSETTINGSDIALOG_H
