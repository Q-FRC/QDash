#include "dialogs/NTSettingsDialog.h"

NTSettingsDialog::NTSettingsDialog(QWidget *parent) : QDialog(parent), Ui::NTSettingsDialog()
{
    setupUi(this);
    checkBox->setChecked(Globals::server.teamNumber);
    lineEdit->setText(QString::fromStdString(Globals::server.server));
    spinBox->setValue(Globals::server.port);
}

NTSettingsDialog::~NTSettingsDialog() {}

void NTSettingsDialog::serializeData() {
    emit dataReady(ServerData{checkBox->checkState() == Qt::Checked, lineEdit->text().toStdString(), spinBox->text().toInt()});
}
