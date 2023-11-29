#include "widgets/DoubleDisplayWidget.h"
#include "stores/TopicStore.h"

#include <QKeyEvent>
#include <QApplication>

DoubleDisplayWidget::DoubleDisplayWidget(const QString &title, const double &defaultValue, const QString &topic) : TextWidget(WidgetTypes::DoubleDisplay, title, QString::number(defaultValue), topic)
{
    m_value = defaultValue;
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

QJsonObject DoubleDisplayWidget::saveObject() {
    QJsonObject object = TextWidget::saveObject();

    object.insert("value", m_value);

    return object;
}

BaseWidget * DoubleDisplayWidget::fromJson(QJsonObject obj) {
    DoubleDisplayWidget *widget = new DoubleDisplayWidget(
        obj.value("title").toString(""),
        obj.value("value").toDouble(0.),
        obj.value("topic").toString(""));

    QFont font;
    font.fromString(obj.value("textFont").toString(qApp->font().toString()));
    widget->setFont(font);

    return widget;
}

void DoubleDisplayWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        m_entry->SetDouble(m_text->text().toDouble());
        m_value = m_text->text().toDouble();
    }
}

