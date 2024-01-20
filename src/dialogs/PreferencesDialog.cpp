#include "dialogs/PreferencesDialog.h"

QMap<QString, QString> StyleSheetMap = {
    {"Dark", ":/dark/stylesheet.qss"},
    {"Dark Purple", ":/dark-purple/stylesheet.qss"},
    {"Light", ":/light/stylesheet.qss"},
    {"Light Purple", ":/light-purple/stylesheet.qss"},
};

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Preferences");

    m_layout = new QFormLayout(this);

    m_choices = new QComboBox(this);
    QStringList choices;
    choices << "Light";
    choices << "Dark";
    choices << "Light Purple";
    choices << "Dark Purple";

    m_choices->addItems(choices);
    m_choices->setCurrentIndex(0);

    m_layout->addRow("Theme", m_choices);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this] {
        emit styleSheetSet(StyleSheetMap.value(m_choices->currentText(), ":/light/stylesheet.qss"));
        accept();
    });

    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    m_layout->addRow(m_buttonBox);
}
