#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <QVariant>

typedef struct Setting {
    const QString Name;
    const QVariant DefaultValue;

    QVariant value() const;
    void setValue(const QVariant &value) const;
} Setting;

namespace Settings {
extern const Setting FirstRun;
extern const Setting RecentFiles;
extern const Setting LoadRecent;
extern const Setting StyleSheet;
}

#endif // CONSTANTS_H
