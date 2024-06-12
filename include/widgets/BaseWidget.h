#pragma once

#include "networktables/NetworkTableEntry.h"
#include "Globals.h"

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>

class BaseWidget : public QWidget
{
    Q_OBJECT
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

    virtual QMenu *constructContextMenu(WidgetData data);

    void paintEvent(QPaintEvent *event);

    virtual void update() {}

signals:
    void resizeRequested(WidgetData data);
    void deleteRequested();
};
