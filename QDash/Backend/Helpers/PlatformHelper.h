// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PLATFORMHELPER_H
#define PLATFORMHELPER_H

#include <QObject>
#include <QQuickWindow>

class PlatformHelper : public QObject {
    Q_OBJECT
public:
    PlatformHelper(QObject *parent = nullptr);

    Q_INVOKABLE void copy(const QString &text);
    Q_INVOKABLE QString baseName(const QString &file);

    Q_INVOKABLE double screenWidth();
    Q_INVOKABLE double screenHeight();

    Q_INVOKABLE double titlebarHeight(QQuickWindow *window);

    Q_INVOKABLE QString filename(const QString &path);

    Q_INVOKABLE bool isMac();
signals:
};

#endif // PLATFORMHELPER_H
