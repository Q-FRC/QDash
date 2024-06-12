#ifndef DOUBLEDIALWIDGET_H
#define DOUBLEDIALWIDGET_H

#include "widgets/NumberDisplayWidget.h"

#include <QDial>

/**
 * @brief Display a double on a dial.
 *
 * Due to limitations within QDial, this only supports up to two decimal places.
 */
class DoubleDialWidget : public NumberDisplayWidget
{
protected:
    QDial *m_dial;

    QPointF m_range;
    int m_fakeValue;

    void keyPressEvent(QKeyEvent *event) override;
public:
    DoubleDialWidget(const QString &title, const double &defaultValue, const QString &topic);
    virtual ~DoubleDialWidget();

    QPointF range();
    void setRange(QPointF range);
    void setRange(double min, double max);

    void update() override;
};

#endif // DOUBLEDIALWIDGET_H
