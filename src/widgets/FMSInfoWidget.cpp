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
    m_event = TopicStore::subscribe(table.toStdString() + "/EventName", this);
    m_controlWord = TopicStore::subscribe(table.toStdString() + "/FMSControlData", this);
    m_gameSpecificMessage = TopicStore::subscribe(table.toStdString() + "/GameSpecificMessage", this);

    m_redAlliance = TopicStore::subscribe(table.toStdString() + "/IsRedAlliance", this);
    m_matchNumber = TopicStore::subscribe(table.toStdString() + "/MatchNumber", this);
    m_matchType = TopicStore::subscribe(table.toStdString() + "/MatchType", this);
    m_allianceStation = TopicStore::subscribe(table.toStdString() + "/StationNumber", this);

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
    TopicStore::unsubscribe(m_topic.toStdString(), this);
    TopicStore::unsubscribe(m_event, this);
    TopicStore::unsubscribe(m_controlWord, this);
    TopicStore::unsubscribe(m_gameSpecificMessage, this);
    TopicStore::unsubscribe(m_redAlliance, this);
    TopicStore::unsubscribe(m_matchNumber, this);
    TopicStore::unsubscribe(m_matchType, this);
    TopicStore::unsubscribe(m_allianceStation, this);
}

void FMSInfoWidget::setValue(const nt::Value &value) {
    m_eventString = QString::fromStdString(m_event->GetString("Unknown"));
    m_eventLabel->setText("Event: " + m_eventString);

    m_gsm = QString::fromStdString(m_gameSpecificMessage->GetString("Unknown"));
    m_gsmLabel->setText("Game Specific Message: " + m_gsm);

    int station = m_allianceStation->GetInteger(1);
    if (!m_redAlliance->GetBoolean(false)) {
        station += 3;
    }

    m_station = (AllianceStation) station;
    m_stationLabel->setText("Station: " + StationNames.value(m_station));

    m_number = m_matchNumber->GetInteger(0);
    m_type = (MatchType) m_matchType->GetInteger(0);

    m_matchLabel->setText("Match: " +
                          MatchTypeNames.value(m_type) +
                          " Match " +
                          QString::number(m_number));

    m_word = (ControlWord) m_controlWord->GetInteger(0);
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

void FMSInfoWidget::forceUpdate() {
    auto placeholder = nt::Value::MakeString("");
    setValue(placeholder);
}
