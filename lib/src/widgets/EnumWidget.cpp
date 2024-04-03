#include "widgets/EnumWidget.h"
#include "stores/TopicStore.h"

EnumWidget::EnumWidget(const QString &topic, const QString &defaultValue, const QString &title) : BaseWidget(WidgetTypes::EnumWidget, title, topic)
{
    setTopic(topic);

    m_value = defaultValue;
    m_colorWidget = new ShapedFrame(Globals::FrameShape::Rectangle, this);

    m_layout->addWidget(m_colorWidget, 1, 0, 3, 1);
    setReady(true);
}

EnumWidget::~EnumWidget() {
    TopicStore::unsubscribe(m_topic.toStdString(), this);
}

QVariantMap EnumWidget::colors() {
    return m_colors;
}

void EnumWidget::setColors(QVariantMap colors) {
    m_colors = colors;
}

Globals::FrameShape EnumWidget::shape() {
    return m_shape;
}

void EnumWidget::setShape(Globals::FrameShape shape) {
    m_shape = shape;
    m_colorWidget->setShape(shape);
}

void EnumWidget::setTopic(const QString &topic) {
    m_topic = topic;
    if (m_entry) TopicStore::unsubscribe(m_topic, this);
    m_entry = TopicStore::subscribe(topic.toStdString(), this, NT_STRING);
}

void EnumWidget::setValue(const nt::Value &value, QString label, bool force) {
    m_value = QString::fromStdString(std::string(value.GetString()));

    if (m_colors.contains(m_value)) m_colorWidget->setColor(m_colors.value(m_value).value<QColor>());
}
