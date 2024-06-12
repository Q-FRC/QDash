#include "widgets/StringDisplayWidget.h"

StringDisplayWidget::StringDisplayWidget(const QString &title, const QString &defaultValue, const QString &topic) : TextWidget(title, defaultValue, topic)
{
    m_value = defaultValue;
}

StringDisplayWidget::~StringDisplayWidget() {
    m_entry.Unpublish();
}

QJsonObject StringDisplayWidget::saveObject() {
    QJsonObject object = TextWidget::saveObject();

    object.insert("value", m_value);
    object.insert("widgetType", (int) WidgetTypes::StringDisplay);

    return object;
}

void StringDisplayWidget::update() {
    if (!m_text->hasFocus()) {
        QString value = QString::fromStdString(m_entry.GetString(m_value.toStdString()));

        m_value = value;
        setText(value);
    }
}

void StringDisplayWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        m_entry.SetString(m_text->text().toStdString());
        m_value = m_text->text();
    }
}
