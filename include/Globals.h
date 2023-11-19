#pragma once

#include "networktables/NetworkTableInstance.h"

#include <QString>
#include <QMap>


typedef struct {
    int tabIdx;
    int row;
    int col;
    int rowSpan;
    int colSpan;
} WidgetData;

enum class WidgetTypes {
    BooleanCheckbox = 0,
    BooleanDisplay = 1,
    StringDisplay = 2,
    DoubleDisplay = 3,
    DoubleDial = 4,
    SendableChooser = 5,
    CameraView = 6,
    EnumWidget = 7
};

Q_DECLARE_FLAGS(WidgetType, WidgetTypes);
Q_DECLARE_OPERATORS_FOR_FLAGS(WidgetType);

enum class TopicTypes {
    SendableChooser,
    Double,
    String,
    Boolean
};

Q_DECLARE_FLAGS(TopicType, TopicTypes);
Q_DECLARE_OPERATORS_FOR_FLAGS(TopicType);

typedef struct {
    bool teamNumber;
    std::string server;
    int port;
} ServerData;

extern bool operator==(const WidgetData &a, const WidgetData &b);

class TypeStore;

namespace Globals {
extern nt::NetworkTableInstance inst;
extern ServerData server;

extern TypeStore *typeStore;

extern QMap<TopicTypes, QString> topicTypeDisplayNames;

extern QStringList ntTopics;
extern QMap<QString, TopicTypes> availableTopics;
extern WidgetData *defaultWidgetData;
}
