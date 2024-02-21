#include "misc/FieldImage.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>

static double FieldWidth = 8.2296;
static double FieldLength = 8.2296 * 2.;

FieldImage::FieldImage(QWidget *parent) : QLabel(parent) {}

void FieldImage::setValue(std::span<const double> value) {
    if (value.size() > 0)
        m_x = value[0];
    if (value.size() > 1)
        m_y = value[1];
    if (value.size() > 2)
        m_theta = value[2];
}

void FieldImage::setRobotWidth(double width) {
    m_width = width;
}

void FieldImage::setRobotLength(double length) {
    m_length = length;
}

void FieldImage::setImage(Globals::File image) {
    m_image = image;
    m_imageChanged = true;

    update();
}

void FieldImage::paintEvent(QPaintEvent *event) {
    QLabel::paintEvent(event);

    int w = event->rect().width();
    int h = event->rect().height() * 4. / 5.;

    // painter setup
    QPainter painter(this);

    if (m_imageChanged || m_lastRect != event->rect()) {
        QPixmap pixmap = QPixmap(m_image.fileName);
        QPixmap pixmap2 = pixmap.scaled(w, h, Qt::KeepAspectRatio);

        m_pixmap = pixmap2;

        m_imageChanged = false;
        m_lastRect = event->rect();
    }

    QPointF topLeft = QPointF(w / 2. - m_pixmap.width() / 2., h / 2. - m_pixmap.height() / 2.);

    painter.drawPixmap(topLeft, m_pixmap, m_pixmap.rect().toRectF());

    m_imageWidth = m_pixmap.width();
    m_imageHeight = m_pixmap.height();

    // meters->pixels
    double meterRatio = (double) m_imageHeight / FieldWidth;

    double robotWidth = m_width * meterRatio;
    double robotLength = m_length * meterRatio;

    // getting some important points & rects
    QPointF robotTopLeft = topLeft + QPointF(m_x * meterRatio, (FieldWidth - m_y) * meterRatio - robotLength);

    QRectF robotRect(robotTopLeft, QSizeF(robotWidth, robotLength));
    QPointF absoluteCenter = robotTopLeft + QPointF(robotWidth / 2., robotLength / 2.);

    // painter setup
    QPen pen(Qt::red, 3);
    painter.setPen(pen);

    // I don't remember geometry class.
    // God Bless the Qt forums
    painter.translate(absoluteCenter);
    painter.rotate(-m_theta); // make CCW positive, algebraic plane
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
