#include "widgets/DoubleDisplayWidget.h"
#include "stores/TopicStore.h"

#include <QKeyEvent>
#include <QApplication>

DoubleDisplayWidget::DoubleDisplayWidget(const QString &topic, const double &defaultValue, const QString &title) : TextWidget(WidgetTypes::DoubleDisplay, topic, QString::number(defaultValue), title)
{
    m_value = defaultValue;
    m_ready = true;
}

DoubleDisplayWidget::~DoubleDisplayWidget() {
    TopicStore::unsubscribe(m_entry, this);
}

void DoubleDisplayWidget::setValue(const nt::Value &value) {
    if (!m_text->hasFocus()) {
        m_value = value.GetDouble();
        setText(QString::number(m_value));
    }
}

void DoubleDisplayWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        m_entry->SetDouble(m_text->text().toDouble());
        m_value = m_text->text().toDouble();
    }
}

