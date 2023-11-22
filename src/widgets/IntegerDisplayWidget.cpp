#include "widgets/IntegerDisplayWidget.h"
#include "stores/TopicStore.h"

#include <QKeyEvent>
#include <QApplication>

IntegerDisplayWidget::IntegerDisplayWidget(const QString &title, const int &defaultValue, const QString &topic) : TextWidget(WidgetTypes::IntegerDisplay, title, QString::number(defaultValue), topic)
{
    m_value = defaultValue;
}

IntegerDisplayWidget::~IntegerDisplayWidget() {
    TopicStore::unsubscribe(m_entry, this);
}

void IntegerDisplayWidget::update() {
    if (!m_text->hasFocus()) {
        int value = m_entry->GetInteger(m_value);

        m_value = value;
        setText(QString::number(value));
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

