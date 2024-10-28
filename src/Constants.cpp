#include "Constants.h"

#include <QSettings>
#include <QGuiApplication>

QVariant Setting::value() const {
    QSettings settings(qApp);
    return settings.value(Name, DefaultValue);
}

void Setting::setValue(const QVariant &value) const {
    QSettings settings(qApp);
    settings.setValue(Name, value);
}

const Setting Settings::RecentFiles{"recentFiles", QStringList{}};
const Setting Settings::LoadRecent{"loadRecent", true};
const Setting Settings::Theme{"theme", "dark"};
const Setting Settings::Accent{"accent", "red"};
