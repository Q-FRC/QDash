#include "widgets/DoubleDisplayWidget.h"
#include "stores/TopicStore.h"

#include <QKeyEvent>
#include <QApplication>

DoubleDisplayWidget::DoubleDisplayWidget(const QString &topic, const double &defaultValue, const QString &title, const bool &ready) : TextWidget(WidgetTypes::DoubleDisplay, topic, QString::number(defaultValue), title)
{
    setTopic(topic);

    m_value = defaultValue;
    setReady(ready);
}

DoubleDisplayWidget::~DoubleDisplayWidget() {
    TopicStore::unsubscribe(m_topic.toStdString(), this);
}

void DoubleDisplayWidget::setTopic(const QString &topic) {


    m_topic = topic;
    if (m_entry) TopicStore::unsubscribe(m_topic, this);
    m_entry = TopicStore::subscribe(topic.toStdString(), this, NT_DOUBLE);
}

void DoubleDisplayWidget::setValue(const nt::Value &value, QString label, bool force) {
    if (!m_text->hasFocus()) {
        m_value = value.GetDouble();
        setText(QString::number(m_value));
    }
}

void DoubleDisplayWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        if (m_entry) m_entry->SetDouble(m_text->text().toDouble());
        m_value = m_text->text().toDouble();
    } else {
        event->ignore();
    }
}

