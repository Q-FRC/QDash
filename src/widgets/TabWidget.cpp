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
    m_maxSize = maxSize;
}