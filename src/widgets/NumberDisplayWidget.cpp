#include "widgets/NumberDisplayWidget.h"

#include <QKeyEvent>

NumberDisplayWidget::NumberDisplayWidget(const QString &title, const double &defaultValue, const QString &topic) : TextWidget(title, QString::number(defaultValue), topic)
{
    m_value = defaultValue;
}

NumberDisplayWidget::~NumberDisplayWidget() {
    m_entry.Unpublish();
}

void NumberDisplayWidget::update() {
    if (!m_text->hasFocus()) {
        double value = m_entry.GetDouble(m_value);

        m_value = value;
        setText(QString::number(value));
    }
}

QJsonObject NumberDisplayWidget::saveObject() {
    QJsonObject object = TextWidget::saveObject();

    object.insert("value", m_value);
    object.insert("widgetType", (int) WidgetTypes::DoubleDisplay);

    return object;
}

void NumberDisplayWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        m_entry.SetDouble(m_text->text().toDouble());
        m_value = m_text->text().toDouble();
    }
}

