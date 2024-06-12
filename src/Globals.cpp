#include "Globals.h"

#include <QStringList>

nt::NetworkTableInstance Globals::inst = nt::NetworkTableInstance::GetDefault();
QString Globals::server = "0.0.0.0";
QStringList Globals::availableTopics{};
