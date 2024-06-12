#ifndef STRINGCHOOSERWIDGET_H
#define STRINGCHOOSERWIDGET_H

#include "widgets/BaseWidget.h"

#include "networktables/NetworkTable.h"

#include <QComboBox>

class StringChooserWidget : public BaseWidget
{
protected:
    QString m_value;

    std::shared_ptr<nt::NetworkTable> m_table;
    nt::NetworkTableEntry m_active;
    nt::NetworkTableEntry m_default;
    nt::NetworkTableEntry m_choices;

    QComboBox *m_chooser;
public:
    StringChooserWidget(const QString &title, const QString &topic);
    virtual ~StringChooserWidget();

    QJsonObject saveObject() override;

//    void update() override;
};

#endif // STRINGCHOOSERWIDGET_H
