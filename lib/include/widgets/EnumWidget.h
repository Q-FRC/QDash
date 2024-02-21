#pragma once

#include "widgets/BaseWidget.h"
#include "misc/ShapedFrame.h"

class EnumWidget : public BaseWidget
{
    Q_OBJECT

    Q_PROPERTY(QString value MEMBER m_value)
    Q_PROPERTY(QString Topic READ topic WRITE setTopic REQUIRED)
    Q_PROPERTY(QVariantMap Colors READ colors WRITE setColors REQUIRED)
    Q_PROPERTY(Globals::FrameShape Shape READ shape WRITE setShape REQUIRED)
protected:
    QString m_value = "";

    ShapedFrame *m_colorWidget;

    QVariantMap m_colors{};
    Globals::FrameShape m_shape;
public:
    EnumWidget(const QString &topic = "", const QString &defaultValue = "", const QString &title = "");
    ~EnumWidget();

    QVariantMap colors();
    void setColors(QVariantMap colors);

    Globals::FrameShape shape();
    void setShape(Globals::FrameShape shape);

    void setTopic(const QString &topic) override;
    void setValue(const nt::Value &value, QString label = "", bool force = false) override;

    inline static WidgetTypes WidgetType = WidgetTypes::EnumWidget;
    inline static TopicTypes TopicType = TopicTypes::String;
    inline static QString SendableName = "";
    inline static QString DisplayName = "Enum";
};
