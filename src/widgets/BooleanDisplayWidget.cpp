#include "widgets/BooleanDisplayWidget.h"
#include "stores/TopicStore.h"

BooleanDisplayWidget::BooleanDisplayWidget(const QString &title, const bool &defaultValue, const QString &topic) : BaseWidget(WidgetTypes::BooleanDisplay, title, topic)
{
    m_value = defaultValue;
    m_colorWidget = new ShapedFrame(Globals::FrameShape::Rectangle, this);

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

Globals::FrameShape BooleanDisplayWidget::shape() {
    return m_shape;
}

void BooleanDisplayWidget::setShape(Globals::FrameShape shape) {
    m_shape = shape;
    m_colorWidget->setShape(shape);
}

QJsonObject BooleanDisplayWidget::saveObject() {
    QJsonObject object = BaseWidget::saveObject();

    object.insert("value", m_value);
    object.insert("trueColor", m_trueColor.name());
    object.insert("falseColor", m_falseColor.name());
    object.insert("shape", Globals::shapeNameMap.key(m_shape));

    return object;
}

BaseWidget * BooleanDisplayWidget::fromJson(QJsonObject obj) {
    BooleanDisplayWidget *widget = new BooleanDisplayWidget(
        obj.value("title").toString(""),
        obj.value("value").toBool(false),
        obj.value("topic").toString(""));

    widget->setTrueColor(QColor::fromString(obj.value("trueColor").toString("#00FF00")));
    widget->setFalseColor(QColor::fromString(obj.value("falseColor").toString("#FF0000")));
    widget->setShape(Globals::shapeNameMap.value(obj.value("shape").toString("Circle")));

    return widget;
}

void BooleanDisplayWidget::setValue(nt::Value value) {
    m_value = value.GetBoolean();

    m_colorWidget->setColor(m_value ? m_trueColor : m_falseColor);
}
