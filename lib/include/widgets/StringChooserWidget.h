#ifndef STRINGCHOOSERWIDGET_H
#define STRINGCHOOSERWIDGET_H

#include "widgets/BaseWidget.h"

#include <QComboBox>

class BaseWidget;

class StringChooserWidget : public BaseWidget
{
    Q_OBJECT

    Q_PROPERTY(QString value MEMBER m_value)
    Q_PROPERTY(QString Topic READ topic WRITE setTopic REQUIRED)
protected:
    QString m_value = "";

    nt::NetworkTableEntry m_active;
    nt::NetworkTableEntry m_default;
    nt::NetworkTableEntry m_choices;
    nt::NetworkTableEntry m_selected;

    QComboBox *m_chooser;

    qsizetype m_flashCounter = 0;

    bool m_readyToUpdate = true;

    QString m_lastSelected = "";
public:
    StringChooserWidget(const QString &topic = "", const QString &defaultValue = "", const QString &title = "");
    ~StringChooserWidget();

    void setTopic(const QString &topic) override;
    void setValue(const nt::Value &value, QString label = "", bool force = false) override;

    void setConnected(bool connected = true) override;

    inline static WidgetTypes WidgetType = WidgetTypes::SendableChooser;
    inline static TopicTypes TopicType = TopicTypes::SendableChooser;
    inline static QString SendableName = "String Chooser";
    inline static QString DisplayName = "Sendable Chooser";

public slots:
    void updateSelected(const QString text);
};

#endif // STRINGCHOOSERWIDGET_H
