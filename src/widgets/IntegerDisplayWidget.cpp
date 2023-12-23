#include "widgets/IntegerDisplayWidget.h"
#include "stores/TopicStore.h"

#include <QKeyEvent>
#include <QApplication>

IntegerDisplayWidget::IntegerDisplayWidget(const QString &topic, const int &defaultValue, const QString &title, const bool &ready) : TextWidget(WidgetTypes::IntegerDisplay, topic, QString::number(defaultValue), title)
{
    m_value = defaultValue;
    setReady(ready);
}

IntegerDisplayWidget::~IntegerDisplayWidget() {
    TopicStore::unsubscribe(m_topic.toStdString(), this);
}

void IntegerDisplayWidget::setValue(const nt::Value &value) {
    if (!m_text->hasFocus()) {
        m_value = value.GetInteger();
        setText(QString::number(m_value));
    }
}

void IntegerDisplayWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        m_entry->SetInteger(m_text->text().toInt());
        m_value = m_text->text().toInt();
    }
}

