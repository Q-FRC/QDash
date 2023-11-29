#include "Globals.h"

#include <QStringList>

nt::NetworkTableInstance Globals::inst = nt::NetworkTableInstance::GetDefault();
ServerData Globals::server{false, "0.0.0.0", NT_DEFAULT_PORT4};

QMap<TopicTypes, QString> Globals::topicTypeDisplayNames = {
    {TopicTypes::SendableChooser, "Sendable Chooser"},
    {TopicTypes::Field2d, "Field2d"},
    {TopicTypes::Double, "Double"},
    {TopicTypes::DoubleArray, "Double Array"},
    {TopicTypes::String, "String"},
    {TopicTypes::Boolean, "Boolean"},
    {TopicTypes::Int, "Integer"}
};

QStringList Globals::ntTopics{};

bool operator==(const WidgetData &a, const WidgetData &b) {
    return (a.row == b.row) &&
           (a.col == b.col) &&
           (a.rowSpan == b.rowSpan) &&
           (a.colSpan == b.rowSpan) &&
           (a.tabIdx == b.tabIdx);
}

QMap<QString, Globals::FrameShape> Globals::shapeNameMap = {
    {"Circle", Globals::FrameShape::Circle},
    {"Triangle", Globals::FrameShape::Triangle},
    {"Rectangle", Globals::FrameShape::Rectangle},
    {"Hexagon", Globals::FrameShape::Hexagon}
};
