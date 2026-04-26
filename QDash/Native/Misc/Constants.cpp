// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

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

void Setting::operator=(const QVariant &value) const
{
    setValue(value);
}

Setting::operator QString() const
{
    return value().toString();
}

Setting::operator bool() const
{
    return value().toBool();
}

Setting::operator QStringList() const
{
    return value().toStringList();
}

Setting::operator double() const
{
    return value().toDouble();
}

Setting::operator int() const
{
    return value().toInt();
}

const Setting Settings::RecentFiles{"recentFiles", QStringList{}};
const Setting Settings::LoadRecent{"loadRecent", true};

const Setting Settings::Theme{"theme", 0};
const Setting Settings::Accent{"accent", 0};
const Setting Settings::Style{"style", "Graphide"};

const Setting Settings::WindowWidth{"windowWidth", 1000};
const Setting Settings::WindowHeight{"windowHeight", 640};
const Setting Settings::WindowX{"windowX", -1};
const Setting Settings::WindowY{"windowY", -1};

const Setting Settings::HannahMontanaMode{"hannahMontanaMode", false};

const Setting Settings::DefaultFontSize{"defaultFontSize", 20};
const Setting Settings::DefaultDisplayFontSize{"defaultDisplayFontSize", 100};
const Setting Settings::DefaultTitleFontSize{"defaultTitleFontSize", 16};

const Setting Settings::Scale{"scale", "1.0"};
const Setting Settings::ResizeToDS{"resizeToDS", "false"};
const Setting Settings::LogLevel{"logLevel", 2};
const Setting Settings::ConnMode{"connMode", 0};
const Setting Settings::TeamNumber{"teamNumber", "0"};
const Setting Settings::IP{"ip", "0.0.0.0"};
const Setting Settings::DisableWidgets{"disableWidgets", true};
