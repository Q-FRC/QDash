#include "widgets/IntegerDisplayWidget.h"
#include "stores/TopicStore.h"

#include <QKeyEvent>
#include <QApplication>

IntegerDisplayWidget::IntegerDisplayWidget(const QString &title, const int &defaultValue, const QString &topic) : TextWidget(WidgetTypes::IntegerDisplay, title, QString::number(defaultValue), topic)
{
    m_value = defaultValue;
    m_ready = true;
}

IntegerDisplayWidget::~IntegerDisplayWidget() {
    TopicStore::unsubscribe(m_entry, this);
}

void IntegerDisplayWidget::setValue(const nt::Value &value) {
    if (!m_text->hasFocus()) {
        m_value = value.GetInteger();
        setText(QString::number(m_value));
    }
}

QJsonObject IntegerDisplayWidget::saveObject() {
    QJsonObject object = TextWidget::saveObject();

    object.insert("value", m_value);

    return object;
}

BaseWidget * IntegerDisplayWidget::fromJson(QJsonObject obj) {
    IntegerDisplayWidget *widget = new IntegerDisplayWidget(
        obj.value("title").toString(""),
        obj.value("value").toInt(0),
        obj.value("topic").toString(""));

    QFont font;
    font.fromString(obj.value("textFont").toString(qApp->font().toString()));
    widget->setFont(font);

    return widget;
}

void IntegerDisplayWidget::keyPressEvent(QKeyEvent *event) {
    if (m_text->hasFocus()) {
        m_entry->SetInteger(m_text->text().toInt());
        m_value = m_text->text().toInt();
    }
}

