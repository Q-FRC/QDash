#include "dialogs/PreferencesDialog.h"

QMap<QString, QString> StyleSheetMap = {
    {"Dark", ":/dark/stylesheet.qss"},
    {"Dark Purple", ":/dark-purple/stylesheet.qss"},
    {"Light", ":/light/stylesheet.qss"},
    {"Light Purple", ":/light-purple/stylesheet.qss"},
};

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent), Ui::PreferencesDialog() {
    setupUi(this);
}

void PreferencesDialog::emitSheet() {
    emit styleSheetSet(StyleSheetMap.value(choices->currentText(), ":/light/stylesheet.qss"));
    accept();
}
