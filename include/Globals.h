#pragma once

#include "networktables/NetworkTableInstance.h"

#include <QString>
#include <QMap>


typedef struct WidgetData {
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
    CameraView = 6
};

extern WidgetData makeWidgetData(int row, int col, int rowSpan, int colSpan, int tabIdx);
extern bool operator==(const WidgetData &a, const WidgetData &b);

namespace Globals {
extern nt::NetworkTableInstance inst;
extern QString server;

enum class TopicTypes {
    SendableChooser,
    Double,
    String,
    Boolean
};

extern QStringList ntTopics;
extern QMap<QString, TopicTypes> availableTopics;
extern WidgetData *defaultWidgetData;
}
