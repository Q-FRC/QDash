#include "widgets/EnumWidget.h"
#include "stores/TopicStore.h"

EnumWidget::EnumWidget(const QString &title, const QString &defaultValue, const QString &topic) : BaseWidget(WidgetTypes::EnumWidget, title, topic)
{
    m_value = defaultValue;
    m_colorWidget = new ShapedFrame(Globals::FrameShape::Rectangle, this);

    m_layout->addWidget(m_colorWidget, 1, 0, 3, 1);
}

EnumWidget::~EnumWidget() {
    TopicStore::unsubscribe(m_entry, this);
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

QJsonObject EnumWidget::saveObject() {
    QJsonObject object = BaseWidget::saveObject();

    object.insert("value", m_value);

    QJsonObject colorMap{};
    QMapIterator<QString, QVariant> iter(m_colors);

    while (iter.hasNext()) {
        iter.next();

        colorMap.insert(iter.key(), iter.value().value<QColor>().name());
    }

    object.insert("colors", colorMap);
    object.insert("shape", Globals::shapeNameMap.key(m_shape));

    return object;
}

BaseWidget * EnumWidget::fromJson(QJsonObject obj) {
    EnumWidget *widget = new EnumWidget(
        obj.value("title").toString(""),
        obj.value("value").toString(""),
        obj.value("topic").toString(""));

    QVariantMap variantColorMap = obj.value("colors").toObject({}).toVariantMap();

    widget->setColors(variantColorMap);
    widget->setShape(Globals::shapeNameMap.value(obj.value("shape").toString("Circle")));
    return widget;
}

void EnumWidget::update() {
    std::string value = m_entry->GetString(m_value.toStdString());

    m_value = QString::fromStdString(value);

    if (m_colors.contains(m_value)) m_colorWidget->setColor(m_colors.value(m_value).value<QColor>());
}
