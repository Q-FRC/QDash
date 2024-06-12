#include "widgets/FieldWidget.h"

#include <QJsonArray>
#include <QResizeEvent>

FieldWidget::FieldWidget(const QString &topic, QVariantList defaultValue, const QString &title, bool fromSendable)
    : BaseWidget(WidgetTypes::Field, title, topic)
{
    m_value = defaultValue;

    m_imageLabel = new FieldImage(this);
    m_imageLabel->setSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Maximum);
    m_imageLabel->setMaximumSize(QSize(size().width(), size().height() * 4. / 5.));
    m_imageLabel->setMinimumSize(1, 1);

    m_layout->addWidget(m_imageLabel, 1, 0, 3, 1, Qt::AlignHCenter);

    if (fromSendable) {
        setTopic(topic + "/Robot");
    }
    m_ready = true;
}

FieldWidget::~FieldWidget() {}

double FieldWidget::robotWidth() {
    return m_width;
}

void FieldWidget::setRobotWidth(double width) {
    m_width = width;
    m_imageLabel->setRobotWidth(width);
}

double FieldWidget::robotLength() {
    return m_length;
}

void FieldWidget::setRobotLength(double length) {
    m_length = length;
    m_imageLabel->setRobotLength(length);
}

Globals::File &FieldWidget::image() {
    return m_image;
}

void FieldWidget::setImage(Globals::File image) {
    m_image = image;
    m_imageLabel->setImage(image);
}

void FieldWidget::setValue(const nt::Value &value) {
    m_value = decltype(m_value)(value.GetDoubleArray().begin(), value.GetDoubleArray().end());

    m_imageLabel->setValue(value.GetDoubleArray());
}

void FieldWidget::resizeEvent(QResizeEvent *event) {
    BaseWidget::resizeEvent(event);

    int w = event->size().width();
    int h = event->size().height() * 4. / 5.;

    m_imageLabel->setPixmap(m_imageLabel->pixmap().scaled(w, h, Qt::KeepAspectRatio));
}
