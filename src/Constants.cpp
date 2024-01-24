#include "Constants.h"

#include <QSettings>
#include <QApplication>

QVariant Setting::value() const {
    QSettings settings(qApp);
    return settings.value(Name, DefaultValue);
}

void Setting::setValue(const QVariant &value) const {
    QSettings settings(qApp);
    settings.setValue(Name, value);
}

const Setting Settings::FirstRun{"firstRun", true};
const Setting Settings::RecentFiles{"recentFiles", QStringList{}};
const Setting Settings::LoadRecent{"loadRecent", false};
const Setting Settings::StyleSheet{"styleSheet", ":/light/stylesheet.qss"};
