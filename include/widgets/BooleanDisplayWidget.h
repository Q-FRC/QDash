#pragma once

#include "widgets/BaseWidget.h"

#include <QFrame>

class BooleanDisplayWidget : public BaseWidget
{
protected:
    bool m_value;

    QFrame *m_colorWidget;

    QColor m_trueColor = Qt::green;
    QColor m_falseColor = Qt::red;
public:
    BooleanDisplayWidget(const QString &title, const bool &defaultValue, const QString &topic);
    virtual ~BooleanDisplayWidget();

    QColor trueColor();
    void setTrueColor(const QColor &color);

    QColor falseColor();
    void setFalseColor(const QColor &color);

    QJsonObject saveObject() override;

    void update();
};
