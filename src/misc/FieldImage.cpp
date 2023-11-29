#include "misc/FieldImage.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>

static double FieldWidth = 8.2296;
static double FieldLength = 8.2296 * 2.;

FieldImage::FieldImage(QWidget *parent) : QLabel(parent) {}

void FieldImage::setValue(std::span<double> value) {
    m_value = value;
}

void FieldImage::setRobotWidth(double width) {
    m_width = width;
}

void FieldImage::setRobotLength(double length) {
    m_length = length;
}

void FieldImage::setImage(Globals::File image) {
    QImage qimage(image.fileName);

    double ratio = qMin(
        (double) parentWidget()->height() / qimage.height(),
        (double) parentWidget()->width() / qimage.width());

    QImage scaled = qimage.scaledToWidth(ratio * qimage.width());

    m_imageWidth = scaled.width();
    m_imageHeight = scaled.height();

    setPixmap(QPixmap::fromImage(scaled));
}

void FieldImage::paintEvent(QPaintEvent *event) {
    QLabel::paintEvent(event);

    if (m_value.size() < 3) return;

    // meters->pixels
    double meterRatio = (double) m_imageHeight / FieldWidth;

    double robotWidth = m_width * meterRatio;
    double robotLength = m_length * meterRatio;

    // getting some important points & rects
    QPointF topLeft(width() / 2. - m_imageWidth / 2.,
                    height() / 2. - m_imageHeight / 2.);

    QPointF robotTopLeft = topLeft + QPointF(m_value[0] * meterRatio, (FieldWidth - m_value[1]) * meterRatio);

    QRectF robotRect(robotTopLeft, QSizeF(robotWidth, robotLength));
    QPointF absoluteCenter = robotTopLeft + QPointF(robotWidth / 2., robotLength / 2.);

    // painter setup
    QPainter painter(this);

    QPen pen(Qt::red, 3);
    painter.setPen(pen);

    // I don't remember geometry class.
    // God Bless the Qt forums
    painter.translate(absoluteCenter);
    painter.rotate(-m_value[2]); // make CCW positive, algebraic plane
    painter.translate(-absoluteCenter);

    painter.drawRect(robotRect);

    QPen arrowPen(Qt::green, 3);
    painter.setPen(arrowPen);

    // ;)
    QPolygonF polygon;
    polygon << robotTopLeft
            << robotTopLeft + QPointF(robotLength, robotWidth / 2.)
            << robotTopLeft + QPointF(0, robotWidth)
            << robotTopLeft;

    painter.drawPolygon(polygon);
}
