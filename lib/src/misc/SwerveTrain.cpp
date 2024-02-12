#include "misc/SwerveTrain.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPen>

SwerveTrain::SwerveTrain(QWidget *parent) : QWidget(parent) {

}

void SwerveTrain::setLocations(QList<double> locations) {
    m_locations = locations;
}

void SwerveTrain::setStates(QList<double> states) {
    m_states = states;
}

void SwerveTrain::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    QRect rect = event->rect();

    int w = rect.width();
    int h = rect.height();

    double l = qMin(w, h);
    double xOffset = qMax(0., (w - h) / 2.);
    double yOffset = qMax(0., (h - w) / 2.);

    QPointF topLeft(xOffset, yOffset);

    QRectF baseRect = QRectF(
        topLeft + QPointF(l / 6., l / 6.),
        QSizeF((l * 2.) / 3.,
               (l * 2.) / 3.)
        );

    QPainter painter(this);

    int penWidth = qCeil(l / 100);

    QPen basePen(QBrush(Qt::white), penWidth);
    painter.setPen(basePen);

    painter.drawRect(baseRect);

    if (m_states.size() != m_locations.size()) return;

    double xMax = 0;
    double yMax = 0;

    for (int i = 0; i < m_locations.size(); ++i) {
        double abs = std::abs(m_locations.at(i));
        if (i % 2 == 0 && abs > xMax) xMax = abs;
        if (i % 2 == 1 && abs > yMax) yMax = abs;
    }

    QPen arcPen(Qt::red);
    QPen velPen(Qt::blue, l / 100.);

    double wheelScale = 8.;

    for (int i = 0; i < m_locations.size(); i += 2) {
        double x = m_locations.at(i);
        double y = m_locations.at(i + 1);

        double px = l / 2. - ( (x * l / 3.) / xMax);
        double py = l / 2. - ( (y * l / 3.) / yMax);

        double r = l / (wheelScale * 2.);

        QPainterPath wheelsPath;

        QRectF wheelRect(
            topLeft + QPointF(px - r, py - r),
            QSizeF(r * 2., r * 2.));

        painter.setPen(basePen);

        wheelsPath.addEllipse(wheelRect);
        painter.drawPath(wheelsPath);
        painter.fillPath(wheelsPath, QBrush(Qt::black));

        // WHEEL ANGLE ARC
        double velocity = m_states.at(i);
        double angle = m_states.at(i + 1) + 90.;

        if (velocity < 0) {
            velocity = qAbs(velocity);
            angle += 180.;
        }

        angle = std::fmod(angle, 360.);
        if (angle < 0) angle += 360.;

        QPainterPath arcPath;

        arcPath.moveTo(wheelRect.center());
        arcPath.arcTo(wheelRect, angle - 10., 20.);
        arcPath.closeSubpath();

        painter.setPen(arcPen);
        painter.drawPath(arcPath);
        painter.fillPath(arcPath, Qt::red);

        // VELOCITY VECTOR ARROW
        QPainterPath velPath;

        painter.setPen(velPen);

        QPointF velI = wheelsPath.pointAtPercent(0.75) + QPointF(0, -l / 100.);
        QPointF velF = velI + QPointF(0., -l / 16. * velocity);

        velPath.moveTo(velI);
        velPath.lineTo(velF);
        velPath.lineTo(velF + QPointF(-l / 32., l / 32.));
        velPath.moveTo(velF);
        velPath.lineTo(velF + QPointF(l / 32., l / 32.));

        // funky rotation thing
        painter.translate(wheelRect.center());
        painter.rotate(-angle + 90);
        painter.translate(-wheelRect.center());

        painter.drawPath(velPath);

        // undo the rotation
        painter.translate(wheelRect.center());
        painter.rotate(angle - 90);
        painter.translate(-wheelRect.center());
    }
}
