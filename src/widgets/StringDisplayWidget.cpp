#include "widgets/StringDisplayWidget.h"

StringDisplayWidget::StringDisplayWidget(const QString &title, const QString &defaultValue, const QString &topic) : TextWidget(title, defaultValue, topic)
{
    m_value = defaultValue;
}

StringDisplayWidget::~StringDisplayWidget() {}

void StringDisplayWidget::update() {
    QString value = QString::fromStdString(m_entry.GetString(m_value.toStdString()));

    m_value = value;
    setText(value);
}
