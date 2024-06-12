#ifndef CommandWidget_H
#define CommandWidget_H

#include <QPushButton>
#include "widgets/BaseWidget.h"

class CommandWidget : public BaseWidget
{
    Q_OBJECT
protected:
    QPushButton *m_button;

    nt::NetworkTableEntry *m_name;
    nt::NetworkTableEntry *m_running;
public:
    CommandWidget(const QString &topic = "", const QString &title = "");
    ~CommandWidget();

    void setTopic(const QString &topic);

    void setValue(const nt::Value &value) override;
};

#endif // CommandWidget_H
