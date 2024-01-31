#include "dialogs/PreferencesDialog.h"
#include "Constants.h"

QMap<QString, QString> StyleSheetMap = {
    {"Dark", ":/dark/stylesheet.qss"},
    {"Dark Purple", ":/dark-purple/stylesheet.qss"},
    {"Light", ":/light/stylesheet.qss"},
    {"Light Purple", ":/light-purple/stylesheet.qss"},
};

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent), Ui::PreferencesDialog() {
    setupUi(this);

    lastLoaded->setChecked(Settings::LoadRecent.value().toBool());
    choices->setCurrentText(StyleSheetMap.key(Settings::StyleSheet.value().toString()));
}

void PreferencesDialog::emitData() {
    emit dataReady(
        StyleSheetMap.value(choices->currentText(), ":/light/stylesheet.qss"),
        lastLoaded->isChecked());
    accept();
}
