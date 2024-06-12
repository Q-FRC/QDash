#include "widgets/TabWidget.h"

// TODO: Implement max size enforcement
TabWidget::TabWidget(const QPoint &maxSize)
{
    m_layout = new QGridLayout(this);

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
