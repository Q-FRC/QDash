#ifndef BOOLEANCHECKBOXWIDGET_H
#define BOOLEANCHECKBOXWIDGET_H

#include "widgets/BaseWidget.h"

#include <QCheckBox>

class BooleanCheckboxWidget : public BaseWidget
{
protected:
    bool m_value;

    QCheckBox *m_checkbox;
public:
    BooleanCheckboxWidget(const QString &title, const bool &defaultValue, const QString &topic);
    virtual ~BooleanCheckboxWidget();

    void update() override;
};

#endif // BOOLEANCHECKBOXWIDGET_H
