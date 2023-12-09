#ifndef TYPESTORE_H
#define TYPESTORE_H

#include "Globals.h"

#include <QObject>
#include <QAction>

class BaseWidget;

class TypeStore : public QObject
{
    Q_OBJECT
private:
    QMultiMap<TopicTypes, WidgetTypes> m_typeWidgetMap{};
    QMap<WidgetTypes, QString> m_widgetNameMap{};
public:
    explicit TypeStore(QWidget *parent = nullptr);

    void registerType(TopicTypes topicType, WidgetTypes widgetType, QString displayName);
    QList<QAction *> generateActionsForType(TopicTypes type, std::string ntTopic);
    QMenu *generateMenuForType(TopicTypes type, std::string ntTopic);

    QString widgetDisplayName(WidgetTypes type);

public slots:
    void emitWidget(BaseWidget *widget, WidgetData data);
signals:
    void widgetReady(BaseWidget *widget, WidgetData data);
};

#endif // TYPESTORE_H
