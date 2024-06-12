#include "stores/TypeStore.h"
#include "widgets/BaseWidget.h"

#include <QMenu>

TypeStore::TypeStore()
{
    // throw std::runtime_error("TypeStore is a static class.");
}

void TypeStore::registerType(TopicTypes topicType, WidgetTypes widgetType, QString displayName) {
    m_typeWidgetMap.insert(topicType, widgetType);
    m_widgetNameMap.insert(widgetType, displayName);
}

QList<QAction *> TypeStore::generateActionsForTopic(Globals::Topic topic, bool emitTopic) {
    QList<QAction *> actions{};

    QList<WidgetTypes> widgetTypes = m_typeWidgetMap.values(topic.type);

    for (WidgetTypes widgetType : widgetTypes) {
        QString displayName = widgetDisplayName(widgetType);

        QAction *action = new QAction(displayName);

        connect(action, &QAction::triggered, action, [this, widgetType, displayName, topic, emitTopic] {
            if (emitTopic) {
                emit topicSelected(topic);
            } else {
                auto widget = BaseWidget::defaultWidgetFromTopic(topic.name, widgetType);

                emit widgetReady(widget, WidgetData{0, 0, 0, 1, 1});
            }
        });

        actions.append(action);
    }

    return actions;
}

QMenu *TypeStore::generateMenuForTopic(Globals::Topic topic, bool emitTopic) {
    QMenu *menu = new QMenu((QWidget *) parent());

    menu->addActions(generateActionsForTopic(topic, emitTopic));

    return menu;
}

QString TypeStore::widgetDisplayName(WidgetTypes type) {
    return m_widgetNameMap.value(type, "");
}

void TypeStore::emitWidget(BaseWidget *widget, WidgetData data) {
    emit widgetReady(widget, data);
}
