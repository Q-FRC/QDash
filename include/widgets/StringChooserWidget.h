#ifndef STRINGCHOOSERWIDGET_H
#define STRINGCHOOSERWIDGET_H

#include "widgets/BaseWidget.h"

#include <QComboBox>

class BaseWidget;

class StringChooserWidget : public BaseWidget
{
    Q_OBJECT

    Q_PROPERTY(QString value MEMBER m_value)
protected:
    QString m_value = "";

    nt::NetworkTableEntry *m_active;
    nt::NetworkTableEntry *m_default;
    nt::NetworkTableEntry *m_choices;
    nt::NetworkTableEntry *m_selected;

    QComboBox *m_chooser;

    qsizetype m_flashCounter = 0;
public:
    StringChooserWidget(const QString &title, const QString &defaultValue, const QString &topic);
    ~StringChooserWidget();

    void setValue(const nt::Value &value) override;

    QJsonObject saveObject() override;
    static BaseWidget * fromJson(QJsonObject obj);
};

#endif // STRINGCHOOSERWIDGET_H
