#ifndef FMSINFOWIDGET_H
#define FMSINFOWIDGET_H

#include "widgets/BaseWidget.h"

enum class MatchType {
    None = 0,
    Practice = 1,
    Qualification = 2,
    Elimination = 3
};

enum class AllianceStation {
    Invalid = 0,
    Red1 = 1,
    Red2 = 2,
    Red3 = 3,

    Blue1 = 4,
    Blue2 = 5,
    Blue3 = 6
};

enum class ControlWord {
    Invalid = 0x0,
    Enabled = 0x1,
    Auto = 0x2,
    Test = 0x4,
    EStop = 0x8,
    FMSAttached = 0x10,
    DSAttached = 0x20
};

Q_DECLARE_FLAGS(ControlFlags, ControlWord)
Q_DECLARE_OPERATORS_FOR_FLAGS(ControlFlags)

extern QMap<MatchType, QString> MatchTypeNames;
extern QMap<AllianceStation, QString> StationNames;

class FMSInfoWidget : public BaseWidget
{
    Q_OBJECT
    Q_PROPERTY(QString Topic READ topic WRITE setTopic REQUIRED)
private:
    int m_number = 0;
    MatchType m_type = MatchType::None;
    ControlWord m_word = ControlWord::Invalid;
    AllianceStation m_station = AllianceStation::Invalid;
    QString m_eventString = "";
    QString m_gsm = "";

    QLabel *m_matchLabel;
    QLabel *m_stationLabel;
    QLabel *m_eventLabel;
    QLabel *m_gsmLabel;

    QLabel *m_dsIconLabel;
    QLabel *m_dsLabel;
    QLabel *m_fmsIconLabel;
    QLabel *m_fmsLabel;

    QLabel *m_controlLabel;

    nt::NetworkTableEntry *m_event;
    nt::NetworkTableEntry *m_controlWord;
    nt::NetworkTableEntry *m_gameSpecificMessage;
    nt::NetworkTableEntry *m_redAlliance;
    nt::NetworkTableEntry *m_matchNumber;
    nt::NetworkTableEntry *m_matchType;
    nt::NetworkTableEntry *m_allianceStation;
public:
    FMSInfoWidget(const QString &table = "", const QString &title = "");
    virtual ~FMSInfoWidget();

    void setTopic(const QString &topic) override;
    void setValue(const nt::Value &value, QString label = "", bool force = false) override;

    inline static WidgetTypes WidgetType = WidgetTypes::FMSInfo;
    inline static TopicTypes TopicType = TopicTypes::FMSInfo;
    inline static QString SendableName = "FMSInfo";
    inline static QString DisplayName = "FMSInfo";
};

#endif // FMSINFOWIDGET_H
