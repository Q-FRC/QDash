#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QVariant>

typedef struct Setting {
    Q_PROPERTY(QString name MEMBER Name CONSTANT FINAL)
    Q_PROPERTY(QVariant value READ value CONSTANT)
    Q_GADGET

public:
    const QString Name;
    const QVariant DefaultValue;

    QVariant value() const;
    void setValue(const QVariant &value) const;

    void operator=(const QVariant &value) const;

    operator QString() const;
    operator bool() const;
    operator QStringList() const;
    operator double() const;
    operator int() const;
} Setting;

namespace Settings {
Q_NAMESPACE
extern const Setting RecentFiles;
extern const Setting LoadRecent;
extern const Setting Theme;
extern const Setting Accent;
extern const Setting Scale;
extern const Setting ResizeToDS;
extern const Setting ConnMode;
extern const Setting TeamNumber;
extern const Setting IP;
}

#endif // CONSTANTS_H
