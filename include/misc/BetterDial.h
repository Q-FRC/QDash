#ifndef BETTERDIAL_H
#define BETTERDIAL_H

#include <QWidget>

class BetterDial : public QWidget
{
    Q_OBJECT
private:
    int m_min = 0;
    int m_max = 0;

    int m_radius = 0;

    int m_value = 0;

    double m_start = 0.;

    bool m_isDragging = false;
public:
    BetterDial(QWidget *parent = nullptr);

    int min();
    void setMin(int min);

    int max();
    void setMax(int max);

    int value();
    void setValue(int value);

    double startingAngle();
    void setStartingAngle(double angle);

    bool isDragging();

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    double valueFromPoint(QPointF point);

signals:
    void sliderMoved(int position);
};

#endif // BETTERDIAL_H
