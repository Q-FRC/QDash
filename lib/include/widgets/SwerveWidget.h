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
    nt::NetworkTableEntry *m_locEntry;
    nt::NetworkTableEntry *m_stateEntry;

    SwerveTrain *m_train;
public:
    SwerveWidget(const QString &topic = "", const QString &title = "");

    void setValue(const nt::Value &value) override;
    void forceUpdate() override;

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
