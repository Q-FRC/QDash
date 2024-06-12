#include "misc/TabMaxSizeDialog.h"

TabMaxSizeDialog::TabMaxSizeDialog(QWidget *parent, const QPoint &maxSize) : QDialog(parent)
{
    m_xBox = new QSpinBox(this);
    m_yBox = new QSpinBox(this);

    m_xBox->setMinimum(0);
    m_yBox->setMinimum(0);

    m_xBox->setValue(maxSize.x());
    m_yBox->setValue(maxSize.y());

    m_layout = new QFormLayout(this);

    m_layout->addRow("Columns", m_xBox);
    m_layout->addRow("Rows", m_yBox);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    connect(m_buttonBox, &QDialogButtonBox::clicked, this, &QDialog::close);

    m_layout->addRow(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this] {
        emit dataReady(QPoint(m_xBox->value(), m_yBox->value()));
    });
}

TabMaxSizeDialog::~TabMaxSizeDialog() {}
