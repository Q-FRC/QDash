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
    DoubleDialWidget(const QString &topic = "", const double &defaultValue = 0., const QString &title = "");
    ~DoubleDialWidget();

    double min();
    void setMin(double min);

    double max();
    void setMax(double max);

    double startingAngle();
    // input degrees
    void setStartingAngle(double angle);

    void setTopic(const QString &topic) override;
    void setValue(const nt::Value &value, QString label = "", bool force = false) override;

    inline static WidgetTypes WidgetType = WidgetTypes::DoubleDial;
    inline static TopicTypes TopicType = TopicTypes::Double;
    inline static QString SendableName = "";
    inline static QString DisplayName = "Dial";
};

#endif // DOUBLEDIALWIDGET_H
