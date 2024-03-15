#include "widgets/StringDisplayWidget.h"
#include "qevent.h"
#include "stores/TopicStore.h"

#include <QApplication>

StringDisplayWidget::StringDisplayWidget(const QString &topic, const QString &defaultValue, const QString &title) : TextWidget(WidgetTypes::StringDisplay, topic, defaultValue, title)
{
    setTopic(topic);

    m_value = defaultValue;
    setReady(true);
}

StringDisplayWidget::~StringDisplayWidget() {
    TopicStore::unsubscribe(m_topic.toStdString(), this);
}

void StringDisplayWidget::setTopic(const QString &topic) {
    

    m_topic = topic;
    if (m_entry) TopicStore::unsubscribe(m_topic, this);
    m_entry = TopicStore::subscribe(topic.toStdString(), this, NT_STRING);
}

void StringDisplayWidget::setValue(const nt::Value &value, QString label, bool force) {
    if (!m_text->hasFocus()) {
        m_value = QString::fromStdString(std::string(value.GetString()));
        setText(m_value);
    }
}

void StringDisplayWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        if (m_entry) m_entry->SetString(m_text->text().toStdString());
        m_value = m_text->text();
    } else {
        event->ignore();
    }
}
