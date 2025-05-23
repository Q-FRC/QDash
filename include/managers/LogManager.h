#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QFile>
#include <QObject>
#include <QQmlEngine>

class LogManager : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT
public:
    explicit LogManager(QObject *parent = nullptr);

    Q_INVOKABLE void log(const QString &level, const QString &subsystem, const QString &message);
    Q_INVOKABLE void info(const QString &subsystem, const QString &message);
    Q_INVOKABLE void warn(const QString &subsystem, const QString &message);
    Q_INVOKABLE void critical(const QString &subsystem, const QString &message);
    Q_INVOKABLE void debug(const QString &subsystem, const QString &message);

private:
    QFile m_logFile;

    static constexpr char m_format[] = "dd.MM.yyyy-hh:mm:ss";
};

#endif // LOGMANAGER_H
