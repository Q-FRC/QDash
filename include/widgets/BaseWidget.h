#pragma once

#include "networktables/NetworkTableEntry.h"

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>

class BaseWidget : public QWidget
{
protected:
    QGridLayout *m_layout;

    QLineEdit *m_title;

    nt::NetworkTableEntry m_entry;
public:
    BaseWidget(const QString &title, const QString &topic);
    virtual ~BaseWidget();

    QFont titleFont();
    void setTitleFont(const QFont &font);

    QString title();
    void setTitle(const QString &title);

    std::string topic();
    void setTopic(const std::string &topic);

    virtual void update() {}
};
