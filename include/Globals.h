#pragma once

#include <networktables/NetworkTableInstance.h>

#include <QString>

// STRUCTS //
typedef struct {
    bool teamNumber;
    std::string server;
    int port;
    QString switchTopic;
} ServerData;

// NAMESPACES //
namespace Globals {
extern nt::NetworkTableInstance inst;
extern ServerData server;
}
