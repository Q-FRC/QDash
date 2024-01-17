#include "dialogs/NTSettingsDialog.h"

#include <QIntValidator>

NTSettingsDialog::NTSettingsDialog(QWidget *parent) : QDialog(parent)
{
    m_addressLabel = new QLabel("Server Address:", this);
    m_addressEdit = new QLineEdit(QString::fromStdString(Globals::server.server), this);

    m_numberLabel = new QLabel("Use Team Number?", this);
    m_numberLabel->setWhatsThis("Whether or not the address you input is your team number.");

    m_numberBox = new QCheckBox(this);
    m_numberBox->setChecked(Globals::server.teamNumber);

    m_portLabel = new QLabel("Port", this);
    m_portEdit = new QLineEdit(QString::number(Globals::server.port), this);

    QIntValidator *validator = new QIntValidator(1, 65535, this);
    m_portEdit->setValidator(validator);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::StandardButton::Ok | QDialogButtonBox::StandardButton::Cancel, this);

    connect(m_buttonBox, &QDialogButtonBox::clicked, this, [this] {
        close();
    });

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this] {
        emit dataReady(ServerData{m_numberBox->checkState() == Qt::Checked, m_addressEdit->text().toStdString(), m_portEdit->text().toInt()});
    });

    m_layout = new QFormLayout(this);

    m_layout->addRow(m_addressLabel, m_addressEdit);
    m_layout->addRow(m_numberLabel, m_numberBox);
    m_layout->addRow(m_portLabel, m_portEdit);
    m_layout->addWidget(m_buttonBox);
}

NTSettingsDialog::~NTSettingsDialog() {}
