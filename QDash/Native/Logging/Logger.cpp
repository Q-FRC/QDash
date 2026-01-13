// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Logger.h"
#include "Misc/Constants.h"

#include <QStandardPaths>
#include <QDesktopServices>

Logger::Logger(QObject *parent) : QObject{parent}
{
    m_dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    m_dir.mkpath(".");
    QString filename = QString("QDash.log");
    QString abs = m_dir.absoluteFilePath(filename);

    if (m_dir.exists(filename)) {
        const QString old = QString("%1.%2").arg(filename, "old");
        m_dir.remove(old);
        m_dir.rename(filename, old);
    }

    m_logFile.setFileName(abs);
}

void Logger::log(const QString &level, const QString &subsystem, const QString &message)
{
    if (!m_logFile.isOpen()) {
        if (!m_logFile.open(QIODevice::Append | QIODevice::WriteOnly)) {
            qCritical() << "Failed to open log file for reading.";
            return;
        }
    }

    // TODO: use fmt instead

    QList<QByteArray> data;
    data << QDateTime::currentDateTime().toString(m_format).toUtf8();
    data << "[" + level.toUtf8().toUpper() + "]";
    data << subsystem.toUtf8() + ":";
    data << message.toUtf8();

    QByteArray toWrite = data.join(" ") + "\n";

    m_logFile.write(toWrite);
    m_logFile.flush();
}

// Log Levels:
// 0: crit
// 1: warn
// 2: info
// 3: debug
void Logger::info(const QString &subsystem, const QString &message)
{
    if (Settings::LogLevel.value().toInt() > 1) {
        QMetaObject::invokeMethod(
            this, [this, subsystem, message]() { log("info", subsystem, message); });
    }
}

void Logger::warn(const QString &subsystem, const QString &message)
{
    if (Settings::LogLevel.value().toInt() > 0) {
        QMetaObject::invokeMethod(
            this, [this, subsystem, message]() { log("warn", subsystem, message); });
    }
}

void Logger::critical(const QString &subsystem, const QString &message)
{
    QMetaObject::invokeMethod(this,
                              [this, subsystem, message]() { log("crit", subsystem, message); });
}

void Logger::debug(const QString &subsystem, const QString &message)
{
    if (Settings::LogLevel.value().toInt() > 2) {
        QMetaObject::invokeMethod(
            this, [this, subsystem, message]() { log("debug", subsystem, message); });
    }
}

void Logger::openLogLocation() {
    QDesktopServices::openUrl(QUrl::fromLocalFile(m_dir.absolutePath()));
}
