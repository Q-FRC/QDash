#include "widgets/NumberDisplayWidget.h"
#include "stores/TopicStore.h"

#include <QKeyEvent>
#include <QApplication>

NumberDisplayWidget::NumberDisplayWidget(const WidgetTypes &type, const QString &title, const double &defaultValue, const QString &topic) : TextWidget(type, title, QString::number(defaultValue), topic)
{
    m_value = defaultValue;
}

NumberDisplayWidget::~NumberDisplayWidget() {
    TopicStore::unsubscribe(m_entry, this);
}

void NumberDisplayWidget::update() {
    if (!m_text->hasFocus()) {
        double value = m_entry->GetDouble(m_value);

        m_value = value;
        setText(QString::number(value));
    }
}

QJsonObject NumberDisplayWidget::saveObject() {
    QJsonObject object = TextWidget::saveObject();

    object.insert("value", m_value);

    return object;
}

BaseWidget * NumberDisplayWidget::fromJson(QJsonObject obj) {
    NumberDisplayWidget *widget = new NumberDisplayWidget(
        WidgetTypes::DoubleDisplay,
        obj.value("title").toString(""),
        obj.value("value").toDouble(0.),
        obj.value("topic").toString(""));

    QFont font;
    font.fromString(obj.value("textFont").toString(qApp->font().toString()));
    widget->setFont(font);

    return widget;
}

void NumberDisplayWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        m_entry->SetDouble(m_text->text().toDouble());
        m_value = m_text->text().toDouble();
    }
}

