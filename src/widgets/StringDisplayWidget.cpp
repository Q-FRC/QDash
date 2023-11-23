#include "widgets/StringDisplayWidget.h"
#include "stores/TopicStore.h"

#include <QApplication>

StringDisplayWidget::StringDisplayWidget(const QString &title, const QString &defaultValue, const QString &topic) : TextWidget(WidgetTypes::StringDisplay, title, defaultValue, topic)
{
    m_value = defaultValue;
}

StringDisplayWidget::~StringDisplayWidget() {
    TopicStore::unsubscribe(m_entry, this);
}

QJsonObject StringDisplayWidget::saveObject() {
    QJsonObject object = TextWidget::saveObject();

    object.insert("value", m_value);

    return object;
}

BaseWidget * StringDisplayWidget::fromJson(QJsonObject obj) {
    StringDisplayWidget *widget = new StringDisplayWidget(
        obj.value("title").toString(""),
        obj.value("value").toString(""),
        obj.value("topic").toString(""));

    QFont font;
    font.fromString(obj.value("textFont").toString(qApp->font().toString()));
    widget->setFont(font);

    return widget;
}

void StringDisplayWidget::setValue(nt::Value value) {
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
