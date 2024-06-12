#include "Globals.h"
#include "stores/TypeStore.h"

#include <QStringList>

nt::NetworkTableInstance Globals::inst = nt::NetworkTableInstance::GetDefault();
ServerData Globals::server{false, "0.0.0.0", NT_DEFAULT_PORT4};

QMap<TopicTypes, QString> Globals::topicTypeDisplayNames{};

QStringList Globals::ntTopics{};

TypeStore Globals::typeStore;

bool operator==(const WidgetData &a, const WidgetData &b) {
    return (a.row == b.row) &&
           (a.col == b.col) &&
           (a.rowSpan == b.rowSpan) &&
           (a.colSpan == b.rowSpan) &&
           (a.tabIdx == b.tabIdx);
}

bool Globals::Topic::operator==(const Globals::Topic &other) const {
    return (this->name == other.name);// && (this->type == other.type);
}

QMap<QString, Globals:: FrameShape> Globals::shapeNameMap = {
    {"Circle", Globals::FrameShape::Circle},
    {"Triangle", Globals::FrameShape::Triangle},
    {"Rectangle", Globals::FrameShape::Rectangle},
    {"Hexagon", Globals::FrameShape::Hexagon}
};

