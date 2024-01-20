#include "Globals.h"
#include "qapplication.h"
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
           (a.colSpan == b.rowSpan);
}

uint Globals::qHash(const Globals::Topic &topic) {
    return qHash(topic.name);
}

bool Globals::Topic::operator==(const Globals::Topic &other) const {
    return (this->name == other.name);
}

bool Globals::GraphXAxis::operator==(const Globals::GraphXAxis &other) const {
    return (other.useTime && this->useTime) || ((other.useTime == this->useTime) && (other.topic == this->topic));
}

QMap<QString, Globals:: FrameShape> Globals::shapeNameMap = {
    {"Circle", Globals::FrameShape::Circle},
    {"Triangle", Globals::FrameShape::Triangle},
    {"Rectangle", Globals::FrameShape::Rectangle},
    {"Hexagon", Globals::FrameShape::Hexagon}
};

void setAppStyleSheet(QString styleSheet) {
    QFile file(styleSheet);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream stream(&file);
    qApp->setStyleSheet(stream.readAll());
}

