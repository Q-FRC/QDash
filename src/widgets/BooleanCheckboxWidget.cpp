#include "widgets/BooleanCheckboxWidget.h"

BooleanCheckboxWidget::BooleanCheckboxWidget(const QString &title, const bool &defaultValue, const QString &topic) : BaseWidget(title, topic)
{
    m_value = defaultValue;

    m_checkbox = new QCheckBox(this);

    m_layout->addWidget(m_title, 0, 0, Qt::AlignHCenter | Qt::AlignCenter);
    m_layout->addWidget(m_checkbox, 1, 0, Qt::AlignHCenter | Qt::AlignCenter);

    connect(m_checkbox, &QCheckBox::stateChanged, this, [this](int state) {
        m_entry.SetBoolean(state == Qt::Checked);
    });
}

BooleanCheckboxWidget::~BooleanCheckboxWidget() {}

void BooleanCheckboxWidget::update() {
    bool value = m_entry.GetBoolean(m_value);

    m_value = value;

    m_checkbox->setCheckState(value ? Qt::Checked : Qt::Unchecked);
}
