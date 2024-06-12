#pragma once

#include "networktables/NetworkTableInstance.h"

#include <QString>

namespace Globals {
    extern nt::NetworkTableInstance inst;
    extern QString server;
    extern QStringList availableTopics;
}
