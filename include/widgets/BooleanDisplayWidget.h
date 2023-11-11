#pragma once

#include <QFrame>

#include "widgets/BaseWidget.h"

class BooleanDisplayWidget : public BaseWidget
{
protected:
    bool m_value;

    QFrame *m_colorWidget;

    QColor m_trueColor = Qt::green;
    QColor m_falseColor = Qt::red;
public:
    BooleanDisplayWidget(const QString &title, const bool &defaultValue, const QString &topic);
    ~BooleanDisplayWidget();

    QColor trueColor();
    void setTrueColor(const QColor &color);

    QColor falseColor();
    void setFalseColor(const QColor &color);

    QJsonObject saveObject() override;

    void update() override;
};
