#pragma once

#include <networktables/NetworkTableInstance.h>

#include <QString>

// STRUCTS //
typedef struct {
    int team;
    std::string ip;
    int mode;
} ServerData;

// NAMESPACES //
namespace Globals {
extern nt::NetworkTableInstance inst;
}
