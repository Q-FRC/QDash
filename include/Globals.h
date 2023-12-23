#pragma once

#include <networktables/NetworkTableInstance.h>

#include <QString>
#include <QMap>
#include <QMetaType>
#include <QMetaEnum>
#include <QFile>
#include <QColor>

// ENUMS //
enum class WidgetTypes {
    BooleanCheckbox = 0,
    BooleanDisplay = 1,
    StringDisplay = 2,
    DoubleDisplay = 3,
    DoubleDial = 4,
    SendableChooser = 5,
    CameraView = 6,
    EnumWidget = 7,
    IntegerDisplay = 8,
    IntegerDial = 9,
    SendableField = 10, // UNUSED BY SAVE/LOAD
    Field = 11,
    Command = 12,
    Graph = 13
};

enum class TopicTypes {
    Double = 0,
    DoubleArray = 1,
    String = 2,
    Boolean = 3,
    Int = 4,

    SendableChooser = 5,
    Field2d = 6,
    Command = 7
};

// STRUCTS //
typedef struct {
    bool teamNumber;
    std::string server;
    int port;
} ServerData;

typedef struct {
    int tabIdx;
    int row;
    int col;
    int rowSpan;
    int colSpan;
} WidgetData;

class TypeStore;


// NAMESPACES //
namespace Globals {

Q_NAMESPACE

enum class FrameShape {
    Circle,
    Triangle,
    Rectangle,
    Hexagon
};

Q_ENUM_NS(FrameShape)

extern nt::NetworkTableInstance inst;
extern ServerData server;

extern TypeStore typeStore;

extern QMap<TopicTypes, QString> topicTypeDisplayNames;

extern QStringList ntTopics;
extern WidgetData *defaultWidgetData;

extern QMap<QString, FrameShape> shapeNameMap;

typedef struct {
    QString fileName;
} File;

typedef struct Topic {
    QString name;
    TopicTypes type;

    bool operator==(const struct Topic &other) const;
} Topic;

extern uint qHash(const Globals::Topic &topic);

typedef struct GraphXAxis {
    bool useTime;
    QString topic;

    bool operator==(const struct GraphXAxis &other) const;
} GraphXAxis;
}

Q_DECLARE_METATYPE(Globals::File)
Q_DECLARE_METATYPE(Globals::Topic)
Q_DECLARE_METATYPE(Globals::GraphXAxis)

namespace CustomMetaTypes {
static const int FrameShape = qMetaTypeId<Globals::FrameShape>();
static const int File = qMetaTypeId<Globals::File>();
static const int Topic = qMetaTypeId<Globals::Topic>();
static const int TopicList = qMetaTypeId<QList<Globals::Topic>>();
static const int XAxis = qMetaTypeId<Globals::GraphXAxis>();
static const int TopicColorMap = qMetaTypeId<QHash<Globals::Topic, QColor>>();
}

extern bool operator==(const WidgetData &a, const WidgetData &b);
// extern bool operator==(const Globals::Topic &a, const Globals::Topic &b);
