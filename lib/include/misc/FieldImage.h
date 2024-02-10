#ifndef FIELDIMAGE_H
#define FIELDIMAGE_H

#include "Globals.h"
#include <QLabel>

class FieldImage : public QLabel
{
    Q_OBJECT
private:
    double m_width = 0.5;
    double m_length = 0.5;

    double m_x = 0.;
    double m_y = 0.;
    double m_theta = 0.;

    double m_imageWidth;
    double m_imageHeight;

    Globals::File m_image{":/2024Field.png"};

    QRect m_lastRect{};
    QPixmap m_pixmap{};
    bool m_imageChanged = false;
public:
    FieldImage(QWidget *parent = nullptr);

    void setValue(std::span<const double> value);
    void setRobotWidth(double width);
    void setRobotLength(double length);

    void setImage(Globals::File image);

    void paintEvent(QPaintEvent *event) override;
};

#endif // FIELDIMAGE_H
