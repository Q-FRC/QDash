#include "widgets/BooleanDisplayWidget.h"
#include "stores/TopicStore.h"

BooleanDisplayWidget::BooleanDisplayWidget(const QString &title, const bool &defaultValue, const QString &topic) : BaseWidget(WidgetTypes::BooleanDisplay, title, topic)
{
    m_value = defaultValue;
    m_colorWidget = new QFrame(this);

    m_layout->addWidget(m_colorWidget, 1, 0, 3, 1);
}

BooleanDisplayWidget::~BooleanDisplayWidget() {
    TopicStore::unsubscribe(m_entry, this);
}

QColor BooleanDisplayWidget::trueColor() {
    return m_trueColor;
}

void BooleanDisplayWidget::setTrueColor(const QColor &color) {
    m_trueColor = color;
}

QColor BooleanDisplayWidget::falseColor() {
    return m_falseColor;
}

void BooleanDisplayWidget::setFalseColor(const QColor &color) {
    m_falseColor = color;
}

QJsonObject BooleanDisplayWidget::saveObject() {
    QJsonObject object = BaseWidget::saveObject();

    object.insert("value", m_value);
    object.insert("trueColor", m_trueColor.name());
    object.insert("falseColor", m_falseColor.name());

    return object;
}

BaseWidget * BooleanDisplayWidget::fromJson(QJsonObject obj) {
    BooleanDisplayWidget *widget = new BooleanDisplayWidget(
        obj.value("title").toString(""),
        obj.value("value").toBool(false),
        obj.value("topic").toString(""));

    widget->setTrueColor(QColor::fromString(obj.value("trueColor").toString("#00FF00")));
    widget->setFalseColor(QColor::fromString(obj.value("falseColor").toString("#FF0000")));

    return widget;
}

void BooleanDisplayWidget::update() {
    bool value = m_entry->GetBoolean(m_value);

    m_value = value;

    m_colorWidget->setStyleSheet("background-color: " + (value ? m_trueColor : m_falseColor).name() + ";");
}
