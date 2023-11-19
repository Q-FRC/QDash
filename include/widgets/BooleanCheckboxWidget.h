#ifndef BOOLEANCHECKBOXWIDGET_H
#define BOOLEANCHECKBOXWIDGET_H

#include <QCheckBox>
#include "widgets/BaseWidget.h"

class BooleanCheckboxWidget : public BaseWidget
{
    Q_OBJECT

    Q_PROPERTY(bool value MEMBER m_value)
    Q_PROPERTY(int Checkbox_Size READ checkboxSize WRITE setCheckboxSize REQUIRED)
protected:
    bool m_value = false;

    int m_checkboxSize = 30;

    QCheckBox *m_checkbox;
public:
    BooleanCheckboxWidget(const QString &title, const bool &defaultValue, const QString &topic);
    ~BooleanCheckboxWidget();

    int checkboxSize();
    void setCheckboxSize(int size);

    QJsonObject saveObject() override;

    void update() override;
};

#endif // BOOLEANCHECKBOXWIDGET_H
