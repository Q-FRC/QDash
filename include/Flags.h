#ifndef QFDFlags_H
#define QFDFlags_H

#include <QFlags>
#include <QObject>

namespace QFDFlags {
    Q_NAMESPACE

enum Direction {
    LEFT = 0x1,
    RIGHT = 0x2,
    TOP = 0x4,
    BOTTOM = 0x8
};

Q_ENUM_NS(Direction);

Q_DECLARE_FLAGS(Directions, Direction);
Q_DECLARE_OPERATORS_FOR_FLAGS(Directions)

enum class ControlWord {
    Invalid = 0x0,
    Enabled = 0x1,
    Auto = 0x2,
    Test = 0x4,
    EStop = 0x8,
    FMSAttached = 0x10,
    DSAttached = 0x20
};

Q_ENUM_NS(ControlWord)

Q_DECLARE_FLAGS(ControlFlags, ControlWord)
Q_DECLARE_OPERATORS_FOR_FLAGS(ControlFlags)
}

#endif // QFDFlags_H
