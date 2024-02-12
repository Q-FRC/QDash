#ifndef SWERVETRAIN_H
#define SWERVETRAIN_H

#include <QWidget>

class SwerveTrain : public QWidget
{
    Q_OBJECT
private:
    QList<double> m_locations = {
        1, 1,
        -1, 1,
        1, -1,
        -1, -1
    };

    QList<double> m_states = {
        5, 0,
        5, 0,
        5, 0,
        5, 0
    };

    void paintEvent(QPaintEvent *event) override;

public:
    SwerveTrain(QWidget *parent = nullptr);

    void setLocations(QList<double> locations);
    void setStates(QList<double> states);
};

#endif // SWERVETRAIN_H
