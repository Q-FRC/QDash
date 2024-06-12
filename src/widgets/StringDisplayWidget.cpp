#include "widgets/StringDisplayWidget.h"

StringDisplayWidget::StringDisplayWidget(const QString &title, const QString &defaultValue, const QString &topic) : TextWidget(title, defaultValue, topic, NT_STRING)
{
    m_value = defaultValue;
}

StringDisplayWidget::~StringDisplayWidget() {}

void StringDisplayWidget::update() {
    QString value = QString::fromStdString(nt::GetString(m_entry, m_value.toStdString()));

    m_value = value;
    setText(value);
}