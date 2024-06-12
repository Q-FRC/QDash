#include "widgets/StringDisplayWidget.h"
#include "stores/TopicStore.h"

#include <QApplication>

StringDisplayWidget::StringDisplayWidget(const QString &topic, const QString &defaultValue, const QString &title) : TextWidget(WidgetTypes::StringDisplay, topic, defaultValue, title)
{
    m_value = defaultValue;
    m_ready = true;
}

StringDisplayWidget::~StringDisplayWidget() {
    TopicStore::unsubscribe(m_entry, this);
}

void StringDisplayWidget::setValue(const nt::Value &value) {
    if (!m_text->hasFocus()) {
        m_value = QString::fromStdString(std::string(value.GetString()));
        setText(m_value);
    }
}

void StringDisplayWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        m_entry->SetString(m_text->text().toStdString());
        m_value = m_text->text();
    }
}
