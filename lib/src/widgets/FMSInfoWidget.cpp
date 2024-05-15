#include "widgets/FMSInfoWidget.h"

#include "stores/TopicStore.h"

QMap<MatchType, QString> MatchTypeNames = {
    {MatchType::Elimination, "Elimination"},
    {MatchType::None, "Unknown"},
    {MatchType::Practice, "Practice"},
    {MatchType::Qualification, "Qual"}
};

QMap<AllianceStation, QString> StationNames = {
    {AllianceStation::Invalid, "None"},
    {AllianceStation::Red1, "Red 1"},
    {AllianceStation::Red2, "Red 2"},
    {AllianceStation::Red3, "Red 3"},
    {AllianceStation::Blue1, "Blue 1"},
    {AllianceStation::Blue2, "Blue 2"},
    {AllianceStation::Blue3, "Blue 3"}
};

FMSInfoWidget::FMSInfoWidget(const QString &table, const QString &title)
    : BaseWidget(WidgetTypes::FMSInfo, title, table, true)
{
    setTopic(table);

    m_matchLabel = new QLabel("Match: Unknown", this);
    m_stationLabel = new QLabel("Station: Unknown", this);
    m_eventLabel = new QLabel("Event: Unknown", this);
    m_gsmLabel = new QLabel("Game Specific Message: Unknown", this);

    m_dsIconLabel = new QLabel(this);
    m_dsLabel = new QLabel("DS Disconnected", this);

    m_fmsIconLabel = new QLabel(this);
    m_fmsLabel = new QLabel("FMS Disconnected", this);

    m_controlLabel = new QLabel("Robot State: Unknown", this);

    for (QLabel *label : QList{
             m_matchLabel,
             m_stationLabel,
             m_eventLabel,
             m_gsmLabel,
             m_dsLabel,
             m_dsLabel,
             m_controlLabel
         }) {
        label->setScaledContents(true);
        label->setWordWrap(true);
    }

    m_layout->addWidget(m_title, 0, 0, 1, 2);

    m_layout->addWidget(m_matchLabel, 1, 0, 1, 2);
    m_layout->addWidget(m_stationLabel, 2, 0, 1, 2);
    m_layout->addWidget(m_eventLabel, 3, 0, 1, 2);
    m_layout->addWidget(m_gsmLabel, 4, 0, 1, 2);

    m_layout->addWidget(m_dsIconLabel, 5, 0);
    m_layout->addWidget(m_dsLabel, 5, 1);

    m_layout->addWidget(m_fmsIconLabel, 6, 0);
    m_layout->addWidget(m_fmsLabel, 6, 1);

    m_layout->addWidget(m_controlLabel, 7, 0, 1, 2);

    m_layout->setColumnStretch(0, 0);
    m_layout->setColumnStretch(1, 1);

    setReady(true);
}

FMSInfoWidget::~FMSInfoWidget() {
    TopicStore::unsubscribe(m_event, this);
    TopicStore::unsubscribe(m_controlWord, this);
    TopicStore::unsubscribe(m_gameSpecificMessage, this);
    TopicStore::unsubscribe(m_redAlliance, this);
    TopicStore::unsubscribe(m_matchNumber, this);
    TopicStore::unsubscribe(m_matchType, this);
    TopicStore::unsubscribe(m_allianceStation, this);
}

void FMSInfoWidget::setTopic(const QString &topic) {
    m_topic = topic;

    TopicStore::unsubscribe(topic.toStdString() + "/EventName", this);
    TopicStore::unsubscribe(topic.toStdString() + "/FMSControlData", this);
    TopicStore::unsubscribe(topic.toStdString() + "/GameSpecificMessage", this);
    TopicStore::unsubscribe(topic.toStdString() + "/IsRedAlliance", this);
    TopicStore::unsubscribe(topic.toStdString() + "/MatchNumber", this);
    TopicStore::unsubscribe(topic.toStdString() + "/MatchType", this);
    TopicStore::unsubscribe(topic.toStdString() + "/StationNumber", this);

    m_event = TopicStore::subscribe(topic.toStdString() + "/EventName", this, NT_STRING, "Event");
    m_controlWord = TopicStore::subscribe(topic.toStdString() + "/FMSControlData", this, NT_INTEGER, "Control");
    m_gameSpecificMessage = TopicStore::subscribe(topic.toStdString() + "/GameSpecificMessage", this, NT_STRING, "GSM");

    m_redAlliance = TopicStore::subscribe(topic.toStdString() + "/IsRedAlliance", this, NT_BOOLEAN, "Red Alliance");
    m_matchNumber = TopicStore::subscribe(topic.toStdString() + "/MatchNumber", this, NT_INTEGER, "Match Number");
    m_matchType = TopicStore::subscribe(topic.toStdString() + "/MatchType", this, NT_INTEGER, "Match Type");
    m_allianceStation = TopicStore::subscribe(topic.toStdString() + "/StationNumber", this, NT_INTEGER, "Station Number");
}

void FMSInfoWidget::setValue(const nt::Value &value, QString label, bool force) {
    if (force) {
        QMap<std::string, QString> map{};
        map.insert("/EventName", "Event");
        map.insert("/FMSControlData", "Control");
        map.insert("/GameSpecificMessage", "GSM");
        map.insert("/IsRedAlliance", "Red Alliance");
        map.insert("/MatchNumber", "Match Number");
        map.insert("/MatchType", "Match Type");
        map.insert("/StationNumber", "StationNumber");

        QMapIterator iter(map);
        while (iter.hasNext()) {
            iter.next();
            TopicStore::updateTopic(m_topic.toStdString() + iter.key(), this, iter.value());
        }

        return;
    }

    if (label == "Event") {
        m_eventString = QString::fromStdString(m_event.GetString("Unknown"));
        m_eventLabel->setText("Event: " + m_eventString);
    }

    if (label == "GSM") {
        m_gsm = QString::fromStdString(m_gameSpecificMessage.GetString("Unknown"));
        m_gsmLabel->setText("Game Specific Message: " + m_gsm);
    }

    if (label == "Station Number" || label == "Red Alliance") {
        int station = m_allianceStation.GetInteger(1);
        if (!m_redAlliance.GetBoolean(false)) {
            station += 3;
        }

        m_station = (AllianceStation) station;
        m_stationLabel->setText("Station: " + StationNames.value(m_station));
    }

    if (label == "Match Number" || label == "Match Type") {
        m_number = m_matchNumber.GetInteger(0);
        m_type = (MatchType) m_matchType.GetInteger(0);

        m_matchLabel->setText("Match: " +
                              MatchTypeNames.value(m_type) +
                              " Match " +
                              QString::number(m_number));
    }

    if (label == "Control") {
        m_word = (ControlWord) m_controlWord.GetInteger(0);
        ControlFlags flags(m_word);

        QString state = "Robot State: ";

        if (flags & ControlWord::Auto) {
            state += "Autonomous";
        }
        else if (flags & ControlWord::Test) {
            state += "Testing";
        } else {
            state += "Teleop";
        }

        if (flags & ControlWord::Enabled) {
            state += " Enabled";
        } else if (flags & ControlWord::EStop) {
            state += " E-Stopped";
        } else {
            state += " Disabled";
        }

        m_controlLabel->setText(state);

        if (flags & ControlWord::FMSAttached) {
            m_fmsLabel->setText("FMS Attached");
            m_fmsIconLabel->setPixmap(QIcon(":/icons/check.svg").pixmap(20, 20));
        } else {
            m_fmsLabel->setText("FMS Disconnected");
            m_fmsIconLabel->setPixmap(QIcon(":/icons/xmark.svg").pixmap(20, 20));
        }

        if (flags & ControlWord::DSAttached) {
            m_dsLabel->setText("DS Attached");
            m_dsIconLabel->setPixmap(QIcon(":/icons/check.svg").pixmap(20, 20));
        } else {
            m_dsLabel->setText("DS Disconnected");
            m_dsIconLabel->setPixmap(QIcon(":/icons/xmark.svg").pixmap(20, 20));
        }
    }
}
