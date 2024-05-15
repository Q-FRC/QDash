#ifndef SWERVEWIDGET_H
#define SWERVEWIDGET_H

#include "widgets/BaseWidget.h"
#include "misc/SwerveTrain.h"

class SwerveWidget : public BaseWidget
{
    Q_OBJECT

    Q_PROPERTY(Globals::DoubleArrayTopic Location_Topic READ locationTopic WRITE setLocationTopic REQUIRED)
    Q_PROPERTY(Globals::DoubleArrayTopic States_Topic READ statesTopic WRITE setStatesTopic REQUIRED)
private:
    nt::NetworkTableEntry m_locEntry;
    nt::NetworkTableEntry m_stateEntry;

    QString m_locTopic = "";
    QString m_stateTopic = "";

    SwerveTrain *m_train;
public:
    SwerveWidget(const QString &topic = "", const QString &title = "");
    virtual ~SwerveWidget();

    void setValue(const nt::Value &value, QString label = "", bool force = false) override;

    void setLocationTopic(const Globals::DoubleArrayTopic &topic);
    Globals::DoubleArrayTopic locationTopic();

    void setStatesTopic(const Globals::DoubleArrayTopic &topic);
    Globals::DoubleArrayTopic statesTopic();

    inline static WidgetTypes WidgetType = WidgetTypes::Swerve;
    inline static TopicTypes TopicType = TopicTypes::None;
    inline static QString SendableName = "";
    inline static QString DisplayName = "Swerve";
};

#endif // SWERVEWIDGET_H
