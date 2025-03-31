#include "managers/LogManager.h"
#include "Constants.h"

#include <QDir>
#include <QStandardPaths>

LogManager::LogManager(QObject *parent)
    : QObject{parent}
{
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    dir.mkpath(".");
    QString filename = QString("QFRCDashboard-%1.log").arg(QDateTime::currentDateTime().toString(m_format));
    m_logFile = dir.absoluteFilePath(filename);
}

void LogManager::log(const QString &level, const QString &subsystem, const QString &message)
{
    QFile f(m_logFile);

    if (!f.open(QIODevice::Append | QIODevice::WriteOnly)) {
        qCritical() << "Failed to open log file for reading.";
        return;
    }

    QList<QByteArray> data;
    data << QDateTime::currentDateTime().toString(m_format).toUtf8();
    data << "[" + level.toUtf8().toUpper() + "]";
    data << subsystem.toUtf8() + ":";
    data << message.toUtf8();

    QByteArray toWrite = data.join(" ") + "\n";

    f.write(toWrite);
    f.close();
}

// Log Levels:
// 0: crit
// 1: warn
// 2: info
// 3: debug
void LogManager::info(const QString &subsystem, const QString &message)
{
    if (Settings::LogLevel.value().toInt() > 1) {
        QMetaObject::invokeMethod(this, [this, subsystem, message] () {
            log("info", subsystem, message);
        });
    }
}

void LogManager::warn(const QString &subsystem, const QString &message)
{
    if (Settings::LogLevel.value().toInt() > 0) {
        QMetaObject::invokeMethod(this, [this, subsystem, message] () {
            log("warn", subsystem, message);
        });
    }
}

void LogManager::critical(const QString &subsystem, const QString &message)
{
    QMetaObject::invokeMethod(this, [this, subsystem, message] () {
        log("crit", subsystem, message);
    });
}

void LogManager::debug(const QString &subsystem, const QString &message)
{
    if (Settings::LogLevel.value().toInt() > 2) {
        QMetaObject::invokeMethod(this, [this, subsystem, message] () {
            log("debug", subsystem, message);
        });
    }
}
