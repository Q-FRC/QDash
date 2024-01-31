#include "misc/GridLineWidget.h"

#include <QPen>
#include <QPainter>
#include <QPainterPath>

GridLineWidget::GridLineWidget(QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);
}

void GridLineWidget::setSize(QPoint size) {
    m_size = size;
}

WidgetData GridLineWidget::selection() {
    return m_selection;
}

void GridLineWidget::setSelection(const WidgetData &selectedIndex) {
    bool doUpdate = m_selection != selectedIndex;

    m_selection = selectedIndex;
    setHasSelection(true);

    if (doUpdate) update();
}

bool GridLineWidget::hasSelection() {
    return m_hasSelection;
}

void GridLineWidget::setHasSelection(const bool &hasSelection) {
    m_hasSelection = hasSelection;
}

bool GridLineWidget::isValidSelection() {
    return m_isValidSelection;
}

void GridLineWidget::setValidSelection(const bool &isValidSelection) {
    m_isValidSelection = isValidSelection;
}

void GridLineWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::gray);
    pen.setWidth(1);
    painter.setPen(pen);

    for (int x = 0; x < m_size.x() + 1; ++x) {
        double xPos = width() / m_size.x() * (double) x;

        painter.drawLine(QLineF(
            QPointF(xPos, 0.),
            QPointF(xPos, height())));
    }

    for (int y = 0; y < m_size.y() + 1; ++y) {
        double yPos = height() / m_size.y() * (double) y;

        painter.drawLine(QLineF(
            QPointF(0, yPos),
            QPointF(width(), yPos)));
    }

    if (m_hasSelection) {
        int row = m_selection.row;
        int col = m_selection.col;
        int rowSpan = m_selection.rowSpan;
        int colSpan = m_selection.colSpan;

        double w = this->width() / m_size.x();
        double h = this->height() / m_size.y();

        double x = w * col;
        double y = h * row;

        pen.setColor(m_isValidSelection ? Qt::green : Qt::red);
        pen.setWidth(6);

        painter.setPen(pen);

        QPainterPath path;

        path.addRect(QRect(
            x, y, w * colSpan, h * rowSpan));

        painter.drawPath(path);
    }
}
