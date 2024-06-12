#include "dialogs/NewDoubleDialDialog.h"

#include "widgets/DoubleDialWidget.h"

NewDoubleDialDialog::NewDoubleDialDialog(std::string ntTopic) : NewWidgetDialog(ntTopic)
{
    m_minInput = new QDoubleSpinBox(this);
    m_minInput->setRange(-1000000., 1000000.);

    m_layout->addRow("Minimum Value", m_minInput);

    m_maxInput = new QDoubleSpinBox(this);
    m_maxInput->setRange(-1000000., 1000000.);

    m_layout->addRow("Maximum Value", m_maxInput);

    m_layout->addWidget(m_buttonBox);

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this, ntTopic] {
        QString name = m_nameInput->text();
        QList<int> data({m_rowInput->value(), m_columnInput->value(), m_rowSpanInput->value(), m_columnSpanInput->value()});

        DoubleDialWidget *widget = new DoubleDialWidget(name, 0., QString::fromStdString(ntTopic));
        widget->setRange(m_minInput->value(), m_maxInput->value());

        emit widgetReady(widget, data);
        close();
    });
}

NewDoubleDialDialog::~NewDoubleDialDialog() {}
