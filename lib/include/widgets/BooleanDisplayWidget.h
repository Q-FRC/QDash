#pragma once

#include "widgets/BaseWidget.h"
#include "misc/ShapedFrame.h"

class BooleanDisplayWidget : public BaseWidget
{
    Q_OBJECT

    Q_PROPERTY(bool value MEMBER m_value)
    Q_PROPERTY(QString Topic READ topic WRITE setTopic REQUIRED)
    Q_PROPERTY(QColor True_Color READ trueColor WRITE setTrueColor REQUIRED)
    Q_PROPERTY(QColor False_Color READ falseColor WRITE setFalseColor REQUIRED)
    Q_PROPERTY(Globals::FrameShape Shape READ shape WRITE setShape REQUIRED)
protected:
    bool m_value = false;

    ShapedFrame *m_colorWidget;

    QColor m_trueColor = Qt::green;
    QColor m_falseColor = Qt::red;
    Globals::FrameShape m_shape = Globals::FrameShape::Rectangle;
public:
    BooleanDisplayWidget(const QString &topic = "", const bool &defaultValue = false, const QString &title = "");
    ~BooleanDisplayWidget();

    QColor trueColor();
    void setTrueColor(const QColor &color);

    QColor falseColor();
    void setFalseColor(const QColor &color);

    Globals::FrameShape shape();
    void setShape(Globals::FrameShape shape);

    void setTopic(const QString &topic) override;
    void setValue(const nt::Value &value, QString label = "", bool force = false) override;

    inline static WidgetTypes WidgetType = WidgetTypes::BooleanDisplay;
    inline static TopicTypes TopicType = TopicTypes::Boolean;
    inline static QString SendableName = "";
    inline static QString DisplayName = "Boolean Display";
};
