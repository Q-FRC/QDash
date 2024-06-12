#include "misc/FieldImage.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>

static double FieldWidth = 8.2296;
static double FieldLength = 8.2296 * 2.;

FieldImage::FieldImage(QWidget *parent) : QLabel(parent) {}

void FieldImage::setValue(std::span<const double> value) {
    m_value = value;
    setImage(m_image); // ensure proper scaling and whatnot
}

void FieldImage::setRobotWidth(double width) {
    m_width = width;
}

void FieldImage::setRobotLength(double length) {
    m_length = length;
}

void FieldImage::setImage(Globals::File image) {
    m_image = image;

    update();
}

void FieldImage::paintEvent(QPaintEvent *event) {
    QLabel::paintEvent(event);

    int w = event->rect().width();
    int h = event->rect().height() * 4. / 5.;

    // painter setup
    QPainter painter(this);

    QPixmap pixmap = QPixmap(m_image.fileName);
    QPixmap pixmap2 = pixmap.scaled(w, h, Qt::KeepAspectRatio);

    if (pixmap.width() != pixmap2.width() || pixmap.height() != pixmap2.height()) {
        pixmap = pixmap2;
        painter.drawPixmap(QPointF(0, h / 2. - pixmap2.height() / 2.), pixmap2, pixmap2.rect().toRectF());
    }

    m_imageWidth = pixmap.width();
    m_imageHeight = pixmap.height();

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
            << robotTopLeft + QPointF(robotWidth, robotLength / 2.)
            << robotTopLeft + QPointF(0, robotLength)
            << robotTopLeft;

    painter.drawPolygon(polygon);
}
