#include "misc/ChartView.h"

ChartView::ChartView(QChart *chart, QWidget *parent) : QChartView(chart, parent){}

void ChartView::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void ChartView::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void ChartView::mouseMoveEvent(QMouseEvent *event) {
    event->ignore();
}

