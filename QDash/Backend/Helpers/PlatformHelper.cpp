// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QClipboard>
#include <QFileInfo>
#include <QGuiApplication>
#include <QScreen>
#include "PlatformHelper.h"

PlatformHelper::PlatformHelper(QObject *parent) : QObject{parent} {}

void PlatformHelper::copy(const QString &text)
{
    qApp->clipboard()->setText(text);
}

QString PlatformHelper::baseName(const QString &file)
{
    QFile f(file);
    QFileInfo info(f);
    return info.baseName();
}

double PlatformHelper::screenWidth()
{
    return qApp->primaryScreen()->availableSize().width();
}

double PlatformHelper::screenHeight()
{
    return qApp->primaryScreen()->availableSize().height();
}

double PlatformHelper::titlebarHeight(QQuickWindow *window)
{
    return window->frameGeometry().height() - window->geometry().height();
}

QString PlatformHelper::filename(const QString& path) {
    return QFileInfo(path).fileName();
}

bool PlatformHelper::isMac()
{
#ifdef Q_OS_APPLE
    return true;
#else
    return false;
#endif
}
