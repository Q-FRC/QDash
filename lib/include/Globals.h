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
    None = 65535,
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
    Graph = 13,
    FMSInfo = 14,
    Swerve = 15
};

enum class TopicTypes {
    None = 65535,
    Double = 0,
    DoubleArray = 1,
    String = 2,
    Boolean = 3,
    Int = 4,

    SendableChooser = 5,
    Field2d = 6,
    Command = 7,
    FMSInfo = 8
};

// STRUCTS //
typedef struct {
    bool teamNumber;
    std::string server;
    int port;
    QString switchTopic;
} ServerData;

typedef struct {
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

typedef struct GraphXAxis {
    bool useTime;
    QString topic;

    bool operator==(const struct GraphXAxis &other) const;
} GraphXAxis;

class NumberTopic;
class DoubleArrayTopic;

class Topic
{
public:
    QString Name;
    TopicTypes Type;

    Topic(QString name = "", TopicTypes type = TopicTypes::None) : Name(name), Type(type) {}

    bool operator==(const Topic &other) const;

    void operator=(const NumberTopic &other);
    void operator=(const DoubleArrayTopic &other);
};

class NumberTopic : public Topic
{
public:
    NumberTopic(QString name = "", TopicTypes type = TopicTypes::None) : Topic(name, type) {}

    void operator=(const Topic &other);
};

class DoubleArrayTopic : public Topic
{
public:
    DoubleArrayTopic(QString name = "") : Topic(name, TopicTypes::DoubleArray) {}

    void operator=(const Topic &other);
};

extern uint qHash(const Topic &topic);

}

Q_DECLARE_METATYPE(Globals::File)
Q_DECLARE_METATYPE(Globals::NumberTopic)
Q_DECLARE_METATYPE(Globals::GraphXAxis)

namespace CustomMetaTypes {
static const int FrameShape = qMetaTypeId<Globals::FrameShape>();
static const int File = qMetaTypeId<Globals::File>();
static const int DATopic = qMetaTypeId<Globals::DoubleArrayTopic>();
static const int NumberTopicList = qMetaTypeId<QList<Globals::NumberTopic>>();
static const int XAxis = qMetaTypeId<Globals::GraphXAxis>();
static const int NumberTopicColorMap = qMetaTypeId<QHash<Globals::NumberTopic, QColor>>();
static const int DoubleArrayTopic = qMetaTypeId<Globals::DoubleArrayTopic>();
}

extern bool operator==(const WidgetData &a, const WidgetData &b);

extern void setAppStyleSheet(QString styleSheet);
