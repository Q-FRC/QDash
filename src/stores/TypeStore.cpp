#include "stores/TypeStore.h"
#include "misc/WidgetDialogGenerator.h"
#include "widgets/BaseWidget.h"

#include <QMenu>

TypeStore::TypeStore(QWidget *parent) : QObject(parent)
{}

void TypeStore::registerType(TopicTypes topicType, WidgetTypes widgetType, QString displayName) {
    m_typeWidgetMap.insert(topicType, widgetType);
    m_widgetNameMap.insert(widgetType, displayName);
}

QList<QAction *> TypeStore::generateActionsForType(TopicTypes type, std::string ntTopic) {
    QList<QAction *> actions{};

    QList<WidgetTypes> widgetTypes = m_typeWidgetMap.values(type);
    for (WidgetTypes widgetType : widgetTypes) {
        QString displayName = widgetDisplayName(widgetType);

        QAction *action = new QAction(displayName);

        connect(action, &QAction::triggered, action, [this, type, widgetType, displayName, ntTopic] {
            auto widget = BaseWidget::defaultWidgetFromTopic(QString::fromStdString(ntTopic), widgetType);

            WidgetDialogGenerator *dialog = new WidgetDialogGenerator(widget);
            dialog->setWindowTitle("New " + displayName + " Widget");

            dialog->show();

            emit dialogShown();

            connect(dialog, &WidgetDialogGenerator::widgetReady, this, &TypeStore::emitWidget);
        });

        actions.append(action);
    }

    return actions;
}

QMenu *TypeStore::generateMenuForType(TopicTypes type, std::string ntTopic) {
    QMenu *menu = new QMenu((QWidget *) parent());

    menu->addActions(generateActionsForType(type, ntTopic));

    return menu;
}

QString TypeStore::widgetDisplayName(WidgetTypes type) {
    return m_widgetNameMap.value(type, "");
}

void TypeStore::emitWidget(BaseWidget *widget, WidgetData data) {
    emit widgetReady(widget, data);
}
