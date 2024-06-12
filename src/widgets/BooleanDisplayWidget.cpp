#include "widgets/BooleanDisplayWidget.h"
#include "stores/TopicStore.h"

BooleanDisplayWidget::BooleanDisplayWidget(const QString &topic, const bool &defaultValue, const QString &title) : BaseWidget(WidgetTypes::BooleanDisplay, title, topic)
{
    m_entry = TopicStore::subscribe(topic.toStdString(), this);

    m_value = defaultValue;
    m_colorWidget = new ShapedFrame(Globals::FrameShape::Rectangle, this);

    m_layout->addWidget(m_colorWidget, 1, 0, 3, 1);
    m_ready = true;
}

BooleanDisplayWidget::~BooleanDisplayWidget() {
    TopicStore::unsubscribe(m_topic.toStdString(), this);
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

void BooleanDisplayWidget::setValue(const nt::Value &value) {
    m_value = value.GetBoolean();

    m_colorWidget->setColor(m_value ? m_trueColor : m_falseColor);
}
