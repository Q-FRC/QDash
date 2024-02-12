#pragma once

#include "networktables/NetworkTableEntry.h"
#include "Globals.h"

#include <QFrame>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QJsonObject>

enum ResizeFlags {
    NONE = 0,
    TOP = 1 << 0,
    BOTTOM = 1 << 1,
    RIGHT = 1 << 2,
    LEFT = 1 << 3
};

Q_DECLARE_FLAGS(ResizeDirection, ResizeFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(ResizeDirection)

class BaseWidget : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString Topic READ topic WRITE setTopic REQUIRED)
protected:
    WidgetTypes m_type;
    QGridLayout *m_layout;

    QLineEdit *m_title;
    bool m_ready = false;

    QString m_topic;

    ResizeDirection m_resize = NONE;

    bool m_sendable = false;

    nt::NetworkTableEntry *m_entry;
public:
    explicit BaseWidget(const WidgetTypes &type = WidgetTypes::StringDisplay, const QString &title = "", const QString &topic = "", const bool sendable = false);
    ~BaseWidget();

    bool ready();
    void setReady(bool ready);

    QFont titleFont();
    void setTitleFont(const QFont &font);

    QString title();
    void setTitle(const QString &title);

    QString topic();
    virtual void setTopic(const QString &topic);

    ResizeDirection resizing();
    void setResizing(ResizeDirection direction);

    virtual QMenu *constructContextMenu(WidgetData data);

    virtual void setValue(const nt::Value &value);
    virtual void forceUpdate();

    void paintEvent(QPaintEvent *event);

    QJsonObject saveObject();
    WidgetData fromJson(QJsonObject obj);

    static BaseWidget *defaultWidgetFromTopic(QString ntTopic, WidgetTypes type);

    inline static WidgetTypes WidgetType = WidgetTypes::None;
    inline static TopicTypes TopicType = TopicTypes::None;
    inline static QString SendableName = "";
    inline static QString DisplayName = "";

signals:
    void reconfigRequested(BaseWidget *widget, WidgetData data);
    void deleteRequested();
    void isReady();
private:
    QVariant readDoubleProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readIntProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readBoolProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readColorProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readMapProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readListProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readFileProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readFontProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readStringProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readShapeProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readTopicListProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readXAxisProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readNumberTopicColorMapProperty(const QMetaProperty &property, const QJsonValue &value);
    QVariant readDoubleArrayTopicProperty(const QMetaProperty &property, const QJsonValue &value);

    QJsonValue writeDoubleProperty(const QMetaProperty &property);
    QJsonValue writeIntProperty(const QMetaProperty &property);
    QJsonValue writeBoolProperty(const QMetaProperty &property);
    QJsonValue writeColorProperty(const QMetaProperty &property);
    QJsonValue writeMapProperty(const QMetaProperty &property);
    QJsonValue writeListProperty(const QMetaProperty &property);
    QJsonValue writeFileProperty(const QMetaProperty &property);
    QJsonValue writeFontProperty(const QMetaProperty &property);
    QJsonValue writeStringProperty(const QMetaProperty &property);
    QJsonValue writeShapeProperty(const QMetaProperty &property);
    QJsonValue writeTopicListProperty(const QMetaProperty &property);
    QJsonValue writeXAxisProperty(const QMetaProperty &property);
    QJsonValue writeNumberTopicColorMapProperty(const QMetaProperty &property);
    QJsonValue writeDoubleArrayTopicProperty(const QMetaProperty &property);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
};
