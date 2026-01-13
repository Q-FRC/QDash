// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LOGGER_H
#define LOGGER_H

#include <QDir>
#include <QFile>
#include <QObject>
#include <QQmlEngine>

class Logger : public QObject {
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
public:
    explicit Logger(QObject *parent = nullptr);

    Q_INVOKABLE void log(const QString &level, const QString &subsystem, const QString &message);
    Q_INVOKABLE void info(const QString &subsystem, const QString &message);
    Q_INVOKABLE void warn(const QString &subsystem, const QString &message);
    Q_INVOKABLE void critical(const QString &subsystem, const QString &message);
    Q_INVOKABLE void debug(const QString &subsystem, const QString &message);

public slots:
    void openLogLocation();

private:
    QFile m_logFile;
    QDir m_dir;

    static constexpr char m_format[] = "dd.MM.yyyy-hh:mm:ss";
};

#endif // LOGGER_H
