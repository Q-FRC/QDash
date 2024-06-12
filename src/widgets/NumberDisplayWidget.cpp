#include "widgets/NumberDisplayWidget.h"

NumberDisplayWidget::NumberDisplayWidget(const QString &title, const double &defaultValue, const QString &topic) : TextWidget(title, QString::number(defaultValue), topic, NT_DOUBLE)
{
    m_value = defaultValue;
}

NumberDisplayWidget::~NumberDisplayWidget() {}

void NumberDisplayWidget::update() {
    double value = nt::GetDouble(m_entry, m_value);

    m_value = value;
    setText(QString::number(value));
}
