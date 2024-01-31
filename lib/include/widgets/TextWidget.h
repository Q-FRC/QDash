#pragma once

#include "widgets/BaseWidget.h"

#include <QLineEdit>

class BaseWidget;

class TextWidget : public BaseWidget
{
    Q_OBJECT
protected:
    QLineEdit *m_text;
public:
    TextWidget(const WidgetTypes &type, const QString &topic, const QString &defaultText, const QString &title);
    ~TextWidget();
    
    QString text();
    void setText(const QString &text);

    QFont font();
    void setFont(const QFont &font);

    QMenu *constructContextMenu(WidgetData data) override;
};
