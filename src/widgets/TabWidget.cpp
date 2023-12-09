#include "widgets/TabWidget.h"
#include "qpainterpath.h"

#include <QPaintEvent>
#include <QPainter>

TabWidget::TabWidget(const QPoint &maxSize, QWidget *parent) : QWidget(parent)
{
    m_layout = new QGridLayout(this);

    // clazy:skip
    installEventFilter(parent);

    setMaxSize(maxSize);
}

TabWidget::~TabWidget() {}

QGridLayout *TabWidget::layout() {
    return m_layout;
}

QPoint TabWidget::maxSize() {
    return m_maxSize;
}

void TabWidget::setMaxSize(const QPoint &maxSize) {
    QPoint lastMax = m_maxSize;
    m_maxSize = maxSize;

    if (lastMax.x() > maxSize.x()) {
        for (int x = maxSize.x(); x < lastMax.x(); ++x) {
            m_layout->setColumnStretch(x, 0);
        }
    } else {
        for (int x = 0; x < m_maxSize.x(); ++x) {
            m_layout->setColumnStretch(x, 1);
        }
    }

    if (lastMax.y() > maxSize.y()) {
        for (int y = maxSize.y(); y < lastMax.y(); ++y) {
            m_layout->setRowStretch(y, 0);
        }
    } else {
        for (int y = 0; y < m_maxSize.y(); ++y) {
            m_layout->setRowStretch(y, 1);
        }
    }
}

WidgetData TabWidget::selectedIndex() {
    return m_selectedIndex;
}

void TabWidget::setSelectedIndex(const WidgetData &selectedIndex) {
    bool doUpdate = m_selectedIndex != selectedIndex;

    m_selectedIndex = selectedIndex;
    setHasSelection(true);

    if (doUpdate) update();
}

bool TabWidget::hasSelection() {
    return m_hasSelection;
}

void TabWidget::setHasSelection(const bool &hasSelection) {
    m_hasSelection = hasSelection;
}

bool TabWidget::isValidSelection() {
    return m_isValidSelection;
}

void TabWidget::setValidSelection(const bool &isValidSelection) {
    m_isValidSelection = isValidSelection;
}

void TabWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::gray);
    pen.setWidth(1);
    painter.setPen(pen);

    for (int x = 0; x < m_maxSize.x(); ++x) {
        double xPos = width() / m_maxSize.x() * x;
        painter.drawLine(QLine(
            QPoint(xPos, 0),
            QPoint(xPos, height())));
    }

    for (int y = 0; y < m_maxSize.y(); ++y) {
        double yPos = height() / m_maxSize.y() * y;
        painter.drawLine(QLine(
            QPoint(0, yPos),
            QPoint(width(), yPos)));
    }

    if (m_hasSelection) {
        int row = m_selectedIndex.row;
        int col = m_selectedIndex.col;
        int rowSpan = m_selectedIndex.rowSpan;
        int colSpan = m_selectedIndex.colSpan;

        double w = this->width() / m_maxSize.x();
        double h = this->height() / m_maxSize.y();

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
