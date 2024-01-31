#include "misc/ShapedFrame.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>

#include <cmath>

/**
 * @brief getLine Get a point a certain length away at a specific angle.
 * @param initial point to start from
 * @param angle radians on the algebraic plane
 * @param length line length (pixels)
 * @return
 */
QPointF getLine(QPointF initial, double angle, double length) {
    double x = initial.x() + length * std::cos(angle);
    double y = initial.y() + length * std::sin(angle);

    return QPointF(x, y);
}

ShapedFrame::ShapedFrame(Globals::FrameShape shape, QWidget *parent) : QFrame(parent)
{
    m_shape = shape;
}

void ShapedFrame::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);

    QPainter painter(this);
    QPainterPath path;
    QRect rect = event->rect();

    int diameter = qMin(rect.width(), rect.height());
    int radius = diameter / 2;

    painter.setBrush(QBrush(m_color));

    switch (m_shape) {
    case Globals::FrameShape::Circle: {
        path.addEllipse(rect.center(), radius, radius);

        painter.drawPath(path);
        painter.fillPath(path, QBrush(m_color));

        return;
    }
    case Globals::FrameShape::Hexagon: {
        // shenanigans
        radius *= std::cos(M_PI / 6.);
        double sideLength = radius * 2. / std::sqrt(3);

        QPointF point1 = QPointF(rect.center().x() - sideLength, rect.height() / 2.);

        QPointF point2 = getLine(point1, M_PI / 3., sideLength);
        QPointF point3 = getLine(point2, 0., sideLength);
        QPointF point4 = getLine(point3, -M_PI / 3., sideLength);
        QPointF point5 = getLine(point4, 4 * M_PI / 3., sideLength);
        QPointF point6 = getLine(point5, M_PI, sideLength);

        QPolygonF polygon;
        polygon << point1 << point2 << point3 << point4 << point5 << point6;

        painter.drawPolygon(polygon);

        return;
    }
    case Globals::FrameShape::Triangle: {
        QPolygonF polygon;
        polygon << QPointF(0, rect.height())
                << QPointF(rect.width() / 2., 0)
                << QPointF(rect.width(), rect.height());

        painter.drawPolygon(polygon);

        return;
    }
    case Globals::FrameShape::Rectangle:
        painter.drawRect(rect);
        painter.fillRect(rect, QBrush(m_color));
        return;
    }
}

ShapedFrame::~ShapedFrame() {}

Globals::FrameShape ShapedFrame::shape() {
    return m_shape;
}

void ShapedFrame::setShape(Globals::FrameShape shape) {
    m_shape = shape;
}

QColor ShapedFrame::color() {
    return m_color;
}

void ShapedFrame::setColor(QColor color) {
    setColor(color.name());
}

void ShapedFrame::setColor(QString color) {
    m_color = color;
}
