#include "widgets/SwerveWidget.h"

#include "stores/TopicStore.h"

#include <QTimer>

SwerveWidget::SwerveWidget(const QString &topic, const QString &title)
    : BaseWidget(WidgetType, topic, title, false) {
    m_train = new SwerveTrain(this);
    m_train->setLocations({1, 1, -1, 1, 1, -1, -1, -1});
    m_train->setStates({0, 0, 0, 0, 0, 0, 0, 0});

    m_layout->addWidget(m_train, 1, 0);
    m_layout->setRowStretch(1, 1);

    setLocationTopic(Globals::DoubleArrayTopic{""});
    setStatesTopic(Globals::DoubleArrayTopic{""});
}

void SwerveWidget::setLocationTopic(const Globals::DoubleArrayTopic &topic) {
    if (m_locEntry) TopicStore::unsubscribe(m_locEntry, this);
    m_locEntry = TopicStore::subscribe(topic.Name.toStdString(), this);
    QTimer::singleShot(1000, this, &SwerveWidget::forceUpdate);
}

Globals::DoubleArrayTopic SwerveWidget::locationTopic() {
    return Globals::DoubleArrayTopic(QString::fromStdString(m_locEntry->GetName()));
}

void SwerveWidget::setStatesTopic(const Globals::DoubleArrayTopic &topic) {
    if (m_stateEntry) TopicStore::unsubscribe(m_stateEntry, this);
    m_stateEntry = TopicStore::subscribe(topic.Name.toStdString(), this);
    QTimer::singleShot(1000, this, &SwerveWidget::forceUpdate);
}

Globals::DoubleArrayTopic SwerveWidget::statesTopic() {
    return Globals::DoubleArrayTopic(QString::fromStdString(m_stateEntry->GetName()));
}

void SwerveWidget::setValue(const nt::Value &value) {
    const nt::Value statesVal = m_stateEntry->GetValue();
    const nt::Value locVal = m_locEntry->GetValue();

    if (statesVal.IsValid()) {
        QList<double> states;
        for (const double state : statesVal.GetDoubleArray()) {
            states.append(state);
        }
        m_train->setStates(states);
    }

    if (locVal.IsValid()) {
        QList<double> locations;
        for (const double location : locVal.GetDoubleArray()) {
            locations.append(location);
        }
        m_train->setLocations(locations);
    }

    m_train->update();
}

void SwerveWidget::forceUpdate() {
    setValue(nt::Value());
}
