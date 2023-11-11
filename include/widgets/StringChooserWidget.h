#ifndef STRINGCHOOSERWIDGET_H
#define STRINGCHOOSERWIDGET_H

#include "widgets/BaseWidget.h"

#include <QComboBox>

class BaseWidget;

class StringChooserWidget : public BaseWidget
{
protected:
    QString m_value;

    nt::NetworkTableEntry *m_active;
    nt::NetworkTableEntry *m_default;
    nt::NetworkTableEntry *m_choices;
    nt::NetworkTableEntry *m_selected;

    QComboBox *m_chooser;

    qsizetype m_flashCounter = 0;
public:
    StringChooserWidget(const QString &title, const QString &topic);
    ~StringChooserWidget();

    QJsonObject saveObject() override;

    void update() override;
};

#endif // STRINGCHOOSERWIDGET_H
