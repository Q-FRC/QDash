#include "widgets/CommandWidget.h"
#include "Globals.h"
#include "stores/TopicStore.h"

#include <QApplication>

CommandWidget::CommandWidget(const QString &topic, const QString &title) : BaseWidget(WidgetTypes::Command, title, topic)
{
    m_name = TopicStore::subscribe(topic.toStdString() + "/.name", this);
    m_running = TopicStore::subscribeWriteOnly(topic.toStdString() + "/running", this);

    m_button = new QPushButton(QString::fromStdString(m_name->GetString("Command")), this);

    connect(m_button, &QPushButton::clicked, this, [this](bool) {
        bool value = m_running->GetBoolean(false);
        m_running->SetBoolean(!value);
    });

    m_layout->addWidget(m_button, 1, 0, 3, 1);

    m_ready = true;
}

CommandWidget::~CommandWidget() {
    TopicStore::unsubscribe(m_name, this);
    TopicStore::unsubscribe(m_running, this);
}

void CommandWidget::setTopic(const QString &topic) {
    if (m_topic == topic)
        return;

    m_topic = topic;

    if (m_name != nullptr) TopicStore::unsubscribe(m_name, this);
    if (m_running != nullptr) TopicStore::unsubscribe(m_running, this);

    m_name = TopicStore::subscribe(topic.toStdString() + "/.name", this);
    m_running = TopicStore::subscribeWriteOnly(topic.toStdString() + "/running", this);
}

void CommandWidget::setValue(const nt::Value &value) {
    if (value.IsString() && value.IsValid()) {
        m_button->setText(QString::fromStdString(std::string{value.GetString()}));
    }
}
