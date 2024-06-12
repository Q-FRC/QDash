#include "widgets/NumberDisplayWidget.h"

NumberDisplayWidget::NumberDisplayWidget(const QString &title, const double &defaultValue, const QString &topic) : TextWidget(title, QString::number(defaultValue), topic)
{
    m_value = defaultValue;
}

NumberDisplayWidget::~NumberDisplayWidget() {}

void NumberDisplayWidget::update() {
    double value = m_entry.GetDouble(m_value);

    m_value = value;
    setText(QString::number(value));
}
