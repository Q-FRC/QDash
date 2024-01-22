#include "dialogs/TabMaxSizeDialog.h"

TabMaxSizeDialog::TabMaxSizeDialog(QWidget *parent, const QPoint &maxSize) : QDialog(parent) , Ui::TabMaxSizeDialog()
{
    setupUi(this);
    xBox->setValue(maxSize.x());
    yBox->setValue(maxSize.y());
}

TabMaxSizeDialog::~TabMaxSizeDialog() {}

void TabMaxSizeDialog::emitData() {
    emit dataReady(QPoint(xBox->value(), yBox->value()));
    accept();
}
