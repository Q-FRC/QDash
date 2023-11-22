#pragma once

#include "widgets/BaseWidget.h"
#include "misc/ShapedFrame.h"

class EnumWidget : public BaseWidget
{
    Q_OBJECT

    Q_PROPERTY(QString value MEMBER m_value)
    Q_PROPERTY(QVariantMap Colors READ colors WRITE setColors REQUIRED)
    Q_PROPERTY(Globals::FrameShape Shape READ shape WRITE setShape REQUIRED)
protected:
    QString m_value = "";

    ShapedFrame *m_colorWidget;

    QVariantMap m_colors{};
    Globals::FrameShape m_shape;
public:
    EnumWidget(const QString &title, const QString &defaultValue, const QString &topic);
    ~EnumWidget();

    QVariantMap colors();
    void setColors(QVariantMap colors);

    Globals::FrameShape shape();
    void setShape(Globals::FrameShape shape);

    QJsonObject saveObject() override;
    static BaseWidget * fromJson(QJsonObject obj);

    void update() override;
};
