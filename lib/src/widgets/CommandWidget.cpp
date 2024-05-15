#include "widgets/CommandWidget.h"
#include "Globals.h"
#include "stores/TopicStore.h"

#include <QApplication>

CommandWidget::CommandWidget(const QString &topic, const QString &title) : BaseWidget(WidgetTypes::Command, title, topic, true)
{
    setTopic(topic);

    m_button = new QPushButton(QString::fromStdString(m_name.GetString("Command")), this);

    connect(m_button, &QPushButton::clicked, this, [this](bool) {
        bool value = m_running.GetBoolean(false);
        m_running.SetBoolean(!value);
    });

    m_layout->addWidget(m_button, 1, 0, 3, 1);

    setReady(true);
}

CommandWidget::~CommandWidget() {
    TopicStore::unsubscribe(m_name, this);
    TopicStore::unsubscribe(m_running, this);
}

void CommandWidget::setTopic(const QString &topic) {
    m_topic = topic;

    TopicStore::unsubscribe(topic.toStdString() + "/.name", this);
    TopicStore::unsubscribe(topic.toStdString() + "/.running", this);

    m_name = TopicStore::subscribe(topic.toStdString() + "/.name", this, NT_STRING, "Name");
    m_running = TopicStore::subscribe(topic.toStdString() + "/running", this, NT_BOOLEAN, "Running", true);
}

void CommandWidget::setValue(const nt::Value &value, QString label, bool force) {
    if (force) {
        QMap<std::string, QString> map{};
        map.insert("/.name", "Name");

        QMapIterator iter(map);
        while (iter.hasNext()) {
            iter.next();
            TopicStore::updateTopic(m_topic.toStdString() + iter.key(), this, iter.value());
        }

        return;
    }

    if (label == "Name") {
        m_button->setText(QString::fromStdString(std::string{value.GetString()}));
    }
}
