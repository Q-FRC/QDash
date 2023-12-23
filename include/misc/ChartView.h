#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>

class ChartView : public QChartView
{
    Q_OBJECT
public:
    ChartView(QChart *chart, QWidget *parent);

    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // CHARTVIEW_H
