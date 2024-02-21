#include "widgets/SwerveWidget.h"

#include "stores/TopicStore.h"

#include <QTimer>

SwerveWidget::SwerveWidget(const QString &topic, const QString &title)
    : BaseWidget(WidgetType, topic, title, false) {
    m_topic = "";

    m_train = new SwerveTrain(this);
    m_train->setLocations({1, 1, -1, 1, 1, -1, -1, -1});
    m_train->setStates({0, 0, 0, 0, 0, 0, 0, 0});

    m_layout->addWidget(m_train, 1, 0);
    m_layout->setRowStretch(1, 1);

    setLocationTopic(Globals::DoubleArrayTopic{""});
    setStatesTopic(Globals::DoubleArrayTopic{""});
}

SwerveWidget::~SwerveWidget() {
    TopicStore::unsubscribe(m_locEntry, this);
    TopicStore::unsubscribe(m_stateEntry, this);
}

void SwerveWidget::setLocationTopic(const Globals::DoubleArrayTopic &topic) {
    if (m_locEntry) TopicStore::unsubscribe(m_locTopic.toStdString(), this);
    m_locEntry = TopicStore::subscribe(topic.Name.toStdString(), this, NT_DOUBLE_ARRAY, "Locations");
    m_locTopic = topic.Name;

    QTimer::singleShot(1000, this, &SwerveWidget::forceUpdate);
}

Globals::DoubleArrayTopic SwerveWidget::locationTopic() {
    return m_locTopic;
}

void SwerveWidget::setStatesTopic(const Globals::DoubleArrayTopic &topic) {
    if (m_stateEntry) TopicStore::unsubscribe(m_stateTopic.toStdString(), this);
    m_stateEntry = TopicStore::subscribe(topic.Name.toStdString(), this, NT_DOUBLE_ARRAY, "States");
    m_stateTopic = topic.Name;

    QTimer::singleShot(1000, this, &SwerveWidget::forceUpdate);
}

Globals::DoubleArrayTopic SwerveWidget::statesTopic() {
    return m_stateTopic;
}

void SwerveWidget::setValue(const nt::Value &value, QString label, bool force) {
    if (force) {
        QMap<std::string, QString> map{};
        map.insert(m_locTopic.toStdString(), "Locations");
        map.insert(m_stateTopic.toStdString(), "States");

        QMapIterator iter(map);
        while (iter.hasNext()) {
            iter.next();
            TopicStore::updateTopic(m_topic.toStdString() + iter.key(), this, iter.value());
        }

        return;
    }

    if (label == "States") {
        QList<double> states;
        for (const double state : value.GetDoubleArray()) {
            states.append(state);
        }
        m_train->setStates(states);
    }

    if (label == "Locations") {
        QList<double> locations;
        for (const double location : value.GetDoubleArray()) {
            locations.append(location);
        }
        m_train->setLocations(locations);
    }

    m_train->update();
}
