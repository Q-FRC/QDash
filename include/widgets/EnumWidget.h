#pragma once

#include <QFrame>

#include "widgets/BaseWidget.h"

class EnumWidget : public BaseWidget
{
    Q_OBJECT

    Q_PROPERTY(QString value MEMBER m_value)
    Q_PROPERTY(QVariantMap Colors READ colors WRITE setColors REQUIRED)
protected:
    QString m_value = "";

    QFrame *m_colorWidget;

    QVariantMap m_colors{};
public:
    EnumWidget(const QString &title, const QString &defaultValue, const QString &topic);
    ~EnumWidget();

    QVariantMap colors();
    void setColors(QVariantMap colors);

    QJsonObject saveObject() override;
    static BaseWidget * fromJson(QJsonObject obj);

    void update() override;
};
