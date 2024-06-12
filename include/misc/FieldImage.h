#ifndef FIELDIMAGE_H
#define FIELDIMAGE_H

#include "Globals.h"
#include <QLabel>

class FieldImage : public QLabel
{
    Q_OBJECT
private:
    std::span<const double> m_value;
    double m_width;
    double m_length;

    double m_imageWidth;
    double m_imageHeight;

    Globals::File m_image;
public:
    FieldImage(QWidget *parent = nullptr);

    void setValue(std::span<const double> value);
    void setRobotWidth(double width);
    void setRobotLength(double length);

    void setImage(Globals::File image);

    void paintEvent(QPaintEvent *event) override;
};

#endif // FIELDIMAGE_H
