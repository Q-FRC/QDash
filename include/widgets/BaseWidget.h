#pragma once

#include "networktables/NetworkTableEntry.h"
#include "Globals.h"

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QJsonObject>

class BaseWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString Topic READ topic WRITE setTopic REQUIRED)
protected:
    WidgetTypes m_type;
    QGridLayout *m_layout;

    QLineEdit *m_title;

    nt::NetworkTableEntry *m_entry;
public:
    BaseWidget(const WidgetTypes &type, const QString &title, const QString &topic);
    ~BaseWidget();

    QFont titleFont();
    void setTitleFont(const QFont &font);

    QString title();
    void setTitle(const QString &title);

    QString topic();
    void setTopic(const QString &topic);

    virtual QMenu *constructContextMenu(WidgetData data);

    virtual void setValue(nt::Value value);

    void paintEvent(QPaintEvent *event);

    virtual QJsonObject saveObject();

    static std::pair<BaseWidget *, WidgetData> fromJson(QJsonObject obj, int tabIdx);
    static BaseWidget *defaultWidgetFromTopic(QString ntTopic, WidgetTypes type);

signals:
    void reconfigRequested(BaseWidget *widget, WidgetData data);
    void deleteRequested();
};
