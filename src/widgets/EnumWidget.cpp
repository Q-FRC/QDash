#include "widgets/EnumWidget.h"
#include "TopicStore.h"

EnumWidget::EnumWidget(const QString &title, const QString &defaultValue, const QString &topic) : BaseWidget(WidgetTypes::EnumWidget, title, topic)
{
    m_value = defaultValue;
    m_colorWidget = new QFrame(this);

    m_layout->addWidget(m_colorWidget, 1, 0, 3, 1);
}

EnumWidget::~EnumWidget() {
    TopicStore::unsubscribe(m_entry, this);
}

QMap<QString, QColor> EnumWidget::colors() {
    return m_colors;
}

void EnumWidget::setColors(QMap<QString, QColor> colors) {
    m_colors = colors;
}

QJsonObject EnumWidget::saveObject() {
    QJsonObject object = BaseWidget::saveObject();

    object.insert("value", m_value);
    object.insert("widgetType", (int) WidgetTypes::BooleanDisplay);

    QJsonObject colorMap{};
    QMapIterator<QString, QColor> iter(m_colors);

    while (iter.hasNext()) {
        iter.next();

        colorMap.insert(iter.key(), iter.value().name());
    }

    object.insert("colors", colorMap);

    return object;
}

void EnumWidget::update() {
    std::string value = m_entry->GetString(m_value.toStdString());

    m_value = QString::fromStdString(value);

    m_colorWidget->setStyleSheet("background-color: " + m_colors.value(m_value).name() + ";");
}
