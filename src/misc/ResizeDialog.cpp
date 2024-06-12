#include "misc/ResizeDialog.h"

ResizeDialog::ResizeDialog(WidgetData widgetData) {
    m_layout = new QFormLayout(this);

    m_rowLabel = new QLabel("Row:", this);
    m_rowInput = new QSpinBox(this);
    m_rowInput->setRange(0, 1000);
    m_rowInput->setValue(widgetData.row);

    m_layout->addRow(m_rowLabel, m_rowInput);

    m_columnLabel = new QLabel("Column:", this);
    m_columnInput = new QSpinBox(this);
    m_columnInput->setRange(0, 1000);
    m_columnInput->setValue(widgetData.col);

    m_layout->addRow(m_columnLabel, m_columnInput);

    m_rowSpanLabel = new QLabel("Row Span:", this);
    m_rowSpanInput = new QSpinBox(this);
    m_rowSpanInput->setRange(0, 1000);
    m_rowSpanInput->setValue(widgetData.rowSpan);

    m_layout->addRow(m_rowSpanLabel, m_rowSpanInput);

    m_columnSpanLabel = new QLabel("Column Span:", this);
    m_columnSpanInput = new QSpinBox(this);
    m_columnSpanInput->setRange(0, 1000);
    m_columnSpanInput->setValue(widgetData.colSpan);

    m_layout->addRow(m_columnSpanLabel, m_columnSpanInput);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    m_layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this] {
        WidgetData data;
        data.row = m_rowInput->value();
        data.col = m_columnInput->value();
        data.rowSpan = m_rowSpanInput->value();
        data.colSpan = m_columnSpanInput->value();
        
        emit resizeReady(data);
        close();
    });

    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::close);
}

ResizeDialog::~ResizeDialog() {}
