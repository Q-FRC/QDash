#ifndef BOOLEANCHECKBOXWIDGET_H
#define BOOLEANCHECKBOXWIDGET_H

#include <QCheckBox>
#include "widgets/BaseWidget.h"

class BooleanCheckboxWidget : public BaseWidget
{
    Q_OBJECT

    Q_PROPERTY(bool value MEMBER m_value)
protected:
    bool m_value = false;

    QCheckBox *m_checkbox;
public:
    BooleanCheckboxWidget(const QString &title, const bool &defaultValue, const QString &topic);
    ~BooleanCheckboxWidget();

    QJsonObject saveObject() override;

    void update() override;
};

#endif // BOOLEANCHECKBOXWIDGET_H
