#include "dialogs/ResizeDialog.h"

ResizeDialog::ResizeDialog(QList<int> widgetData) {
    m_layout = new QFormLayout(this);

    m_rowLabel = new QLabel("Row:", this);
    m_rowInput = new QSpinBox(this);
    m_rowInput->setRange(0, 1000);
    m_rowInput->setValue(widgetData[1]);

    m_layout->addRow(m_rowLabel, m_rowInput);

    m_columnLabel = new QLabel("Column:", this);
    m_columnInput = new QSpinBox(this);
    m_columnInput->setRange(0, 1000);
    m_columnInput->setValue(widgetData[2]);

    m_layout->addRow(m_columnLabel, m_columnInput);

    m_rowSpanLabel = new QLabel("Row Span:", this);
    m_rowSpanInput = new QSpinBox(this);
    m_rowSpanInput->setRange(0, 1000);
    m_rowSpanInput->setValue(widgetData[3]);

    m_layout->addRow(m_rowSpanLabel, m_rowSpanInput);

    m_columnSpanLabel = new QLabel("Column Span:", this);
    m_columnSpanInput = new QSpinBox(this);
    m_columnSpanInput->setRange(0, 1000);
    m_columnSpanInput->setValue(widgetData[4]);

    m_layout->addRow(m_columnSpanLabel, m_columnSpanInput);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    m_layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, [this]() {
        QList<int> data({m_rowInput->value(), m_columnInput->value(), m_rowSpanInput->value(), m_columnSpanInput->value()});
        emit finished(data);
        close();
    });

    connect(m_buttonBox, &QDialogButtonBox::rejected, m_buttonBox, &QDialogButtonBox::close);
}

ResizeDialog::~ResizeDialog() {}
