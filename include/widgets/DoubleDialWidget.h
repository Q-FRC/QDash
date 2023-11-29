#ifndef DOUBLEDIALWIDGET_H
#define DOUBLEDIALWIDGET_H

#include "widgets/DoubleDisplayWidget.h"
#include "misc/BetterDial.h"

#include <QDial>

class BaseWidget;

/**
 * @brief Display a double on a dial.
 *
 * Due to limitations within QDial, this only supports up to two decimal places.
 */
class DoubleDialWidget : public DoubleDisplayWidget
{
    Q_OBJECT

    Q_PROPERTY(double value MEMBER m_value)
    Q_PROPERTY(double Maximum READ max WRITE setMax REQUIRED)
    Q_PROPERTY(double Minimum READ min WRITE setMin REQUIRED)
    Q_PROPERTY(double Starting_Angle READ startingAngle WRITE setStartingAngle REQUIRED)
protected:
    BetterDial *m_dial;

    double m_min = 0;
    double m_max = 360.;

    int m_fakeValue = 0;

    double m_startingAngle = 180.;

    void keyPressEvent(QKeyEvent *event) override;
public:
    DoubleDialWidget(const QString &title, const double &defaultValue, const QString &topic);
    ~DoubleDialWidget();

    double min();
    void setMin(double min);

    double max();
    void setMax(double max);

    double startingAngle();
    // input degrees
    void setStartingAngle(double angle);

    void setValue(const nt::Value &value) override;

    QJsonObject saveObject() override;
    static BaseWidget * fromJson(QJsonObject obj);
};

#endif // DOUBLEDIALWIDGET_H
