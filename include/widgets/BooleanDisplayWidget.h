#pragma once

#include <QFrame>

#include "widgets/BaseWidget.h"

class BooleanDisplayWidget : public BaseWidget
{
    Q_OBJECT

    Q_PROPERTY(bool value MEMBER m_value)
    Q_PROPERTY(QColor True_Color READ trueColor WRITE setTrueColor REQUIRED)
    Q_PROPERTY(QColor False_Color READ falseColor WRITE setFalseColor REQUIRED)
protected:
    bool m_value = false;

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
