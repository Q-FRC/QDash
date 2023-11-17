#pragma once

#include <QFrame>

#include "widgets/BaseWidget.h"

class EnumWidget : public BaseWidget
{
protected:
    QString m_value;

    QFrame *m_colorWidget;

    QMap<QString, QColor> m_colors;
public:
    EnumWidget(const QString &title, const QString &defaultValue, const QString &topic);
    ~EnumWidget();

    QMap<QString, QColor> colors();
    void setColors(QMap<QString, QColor> colors);

    QJsonObject saveObject() override;

    void update() override;
};
