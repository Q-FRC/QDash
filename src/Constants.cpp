#include "Constants.h"

#include <QGuiApplication>
#include <QSettings>

QVariant Setting::value() const
{
    QSettings settings(qApp);
    return settings.value(Name, DefaultValue);
}

void Setting::setValue(const QVariant &value) const
{
    QSettings settings(qApp);
    settings.setValue(Name, value);
}

const Setting Settings::RecentFiles{"recentFiles", QStringList{}};
const Setting Settings::LoadRecent{"loadRecent", true};
const Setting Settings::Theme{"theme", "dark"};
const Setting Settings::Accent{"accent", "red"};
const Setting Settings::Scale{"scale", "1.0"};
const Setting Settings::ResizeToDS{"resizeToDS", "false"};
