#include "logging/Logger.h"
#include "misc/Constants.h"

#include <QDir>
#include <QStandardPaths>

Logger::Logger(QObject *parent)
    : QObject{parent}
{
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    dir.mkpath(".");
    QString filename = QString("QDash-%1.log").arg(QDateTime::currentDateTime().toString(m_format));
    m_logFile.setFileName(dir.absoluteFilePath(filename));
}

void Logger::log(const QString &level, const QString &subsystem, const QString &message)
{
    if (!m_logFile.isOpen()) {
        if (!m_logFile.open(QIODevice::Append | QIODevice::WriteOnly)) {
            qCritical() << "Failed to open log file for reading.";
            return;
        }
    }

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
        QMetaObject::invokeMethod(this, [this, subsystem, message] () {
            log("info", subsystem, message);
        });
    }
}

void Logger::warn(const QString &subsystem, const QString &message)
{
    if (Settings::LogLevel.value().toInt() > 0) {
        QMetaObject::invokeMethod(this, [this, subsystem, message] () {
            log("warn", subsystem, message);
        });
    }
}

void Logger::critical(const QString &subsystem, const QString &message)
{
    QMetaObject::invokeMethod(this, [this, subsystem, message] () {
        log("crit", subsystem, message);
    });
}

void Logger::debug(const QString &subsystem, const QString &message)
{
    if (Settings::LogLevel.value().toInt() > 2) {
        QMetaObject::invokeMethod(this, [this, subsystem, message] () {
            log("debug", subsystem, message);
        });
    }
}
