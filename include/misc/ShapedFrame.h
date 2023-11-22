#ifndef SHAPEDFRAME_H
#define SHAPEDFRAME_H

#include "Globals.h"

#include <QFrame>
#include <QMetaType>

class ShapedFrame : public QFrame
{
    Q_OBJECT
public:

    ShapedFrame(Globals::FrameShape shape = Globals::FrameShape::Circle, QWidget *parent = nullptr);
    virtual ~ShapedFrame();

    Globals::FrameShape shape();
    void setShape(Globals::FrameShape shape);

    QColor color();
    void setColor(QColor color);
    void setColor(QString color);

    void paintEvent(QPaintEvent *event) override;
private:
    Globals::FrameShape m_shape;

    QColor m_color;
};

#endif // SHAPEDFRAME_H
