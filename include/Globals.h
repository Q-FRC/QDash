#pragma once

#include "networktables/NetworkTableInstance.h"

#include <QString>
#include <QMap>

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
}
