#include "misc/BetterDial.h"

#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QPolygonF>
#include <QPointF>

#include <cmath>

BetterDial::BetterDial(QWidget *parent)
    : QWidget{parent}
{
    setMin(0);
    setMax(10);
    setStartingAngle(0.);

    removeEventFilter(parent);
}

int BetterDial::min() {
    return m_min;
}

void BetterDial::setMin(int min) {
    m_min = min;
}

int BetterDial::max() {
    return m_max;
}

void BetterDial::setMax(int max) {
    m_max = max;
}

int BetterDial::value() {
    return m_value;
}

void BetterDial::setValue(int value) {
    m_value = value;
}

double BetterDial::startingAngle() {
    return m_start;
}

void BetterDial::setStartingAngle(double angle) {
    m_start = angle;
}

bool BetterDial::isDragging() {
    return m_isDragging;
}

void BetterDial::paintEvent(QPaintEvent *event) {

    QWidget::paintEvent(event);

    QPainter painter(this);
    QPainterPath path;
    QRect rect = event->rect();

    int diameter = qMin(rect.width(), rect.height());
    int radius = diameter / 2;

    m_radius = radius;

    // Background Circle
    painter.setBrush(QBrush(Qt::white));

    path.addEllipse(rect.center(), radius, radius);

    painter.drawPath(path);
    painter.fillPath(path, QBrush(Qt::white));

    // Center Circle
    painter.setBrush(QBrush(Qt::red));

    QPainterPath centerPath;
    centerPath.addEllipse(rect.center(), 10., 10.);

    painter.drawPath(centerPath);
    painter.fillPath(centerPath, QBrush(Qt::red));

    // Triangle Thing
    double range = max() - min();
    double rotations = m_value / range;
    double angle = rotations * 2 * M_PI - m_start;

    double x = rect.center().x() + radius * std::sin(angle);
    double y = rect.center().y() + radius * std::cos(angle);

    double perpendicularSin = std::sin(angle + M_PI / 2.);
    double perpendicularCos = std::cos(angle + M_PI / 2.);

    QPolygonF polygon;
    polygon << QPointF(rect.center().x() + perpendicularSin * 10., rect.center().y() + perpendicularCos * 10.)
            << QPointF(x + perpendicularSin, y + perpendicularCos)
            << QPointF(x - perpendicularSin, y - perpendicularCos)
            << QPointF(rect.center().x() - perpendicularSin * 10., rect.center().y() - perpendicularCos * 10.);

    painter.drawPolygon(polygon);

    return;
}

void BetterDial::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);

    if (std::abs((event->pos() - rect().center()).manhattanLength()) >= m_radius) return event->ignore();

    if (event->buttons() & Qt::LeftButton) {
        setValue(valueFromPoint(event->position()));
        emit sliderMoved(m_value);

        m_isDragging = true;
        event->accept();
    }
}

void BetterDial::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);

    if (std::abs((event->pos() - rect().center()).manhattanLength()) >= m_radius) return event->ignore();

    if (event->buttons() & Qt::LeftButton) {
        setValue(valueFromPoint(event->position()));
        emit sliderMoved(m_value);

        m_isDragging = true;

        event->accept();
    }
}

void BetterDial::mouseReleaseEvent(QMouseEvent *event) {
    if (std::abs((event->pos() - rect().center()).manhattanLength()) >= m_radius) return event->ignore();

    m_isDragging = false;
}

double BetterDial::valueFromPoint(QPointF point) {
    double x = -(point.x() - this->rect().center().x());
    double y = -(point.y() - this->rect().center().y());

    double angle = std::fmod((std::atan2(x, y) + M_PI + m_start), (2 * M_PI));
    double rotations = angle / (2 * M_PI);

    double range = max() - min();

    double offset = rotations * range;

    return min() + offset;
}
