#ifndef DOUBLEDIALWIDGET_H
#define DOUBLEDIALWIDGET_H

#include "widgets/NumberDisplayWidget.h"

#include <QDial>

class BaseWidget;

/**
 * @brief Display a double on a dial.
 *
 * Due to limitations within QDial, this only supports up to two decimal places.
 */
class DoubleDialWidget : public NumberDisplayWidget
{
    Q_OBJECT

    Q_PROPERTY(double value MEMBER m_value)
    Q_PROPERTY(double Maximum READ max WRITE setMax REQUIRED)
    Q_PROPERTY(double Minimum READ min WRITE setMin REQUIRED)
protected:
    QDial *m_dial;

    double m_min = 0;
    double m_max = 360.;

    int m_fakeValue = 0;

    void keyPressEvent(QKeyEvent *event) override;
public:
    DoubleDialWidget(const QString &title, const double &defaultValue, const QString &topic);
    ~DoubleDialWidget();

    double min();
    void setMin(double min);

    double max();
    void setMax(double max);

    QJsonObject saveObject() override;
    static BaseWidget * fromJson(QJsonObject obj);

    void update() override;
};

#endif // DOUBLEDIALWIDGET_H
