#include "SettingsManager.h"
#include "Constants.h"
#include "Globals.h"

SettingsManager::SettingsManager(LogManager *logs, QObject *parent)
    : QObject{parent}
    , m_logs(logs)
{}

// TODO: Seems like this reconnect happens thrice (once for each param)
// pls fix
void SettingsManager::reconnectServer()
{
    std::string server = QString(Settings::IP).toStdString();
    int team = Settings::TeamNumber;
    int mode = Settings::ConnMode;

    switch (mode) {
    // IP Address
    case 0:
        Globals::inst.SetServer(server.c_str(), NT_DEFAULT_PORT4);
        m_logs->info("NT", "Requested connect to IP " + QString::fromStdString(server));
        break;
    // Team Number
    case 1:
        Globals::inst.SetServerTeam(team, NT_DEFAULT_PORT4);
        m_logs->info("NT", "Requested connect to team number " + QString::number(team));
        break;
    // DS&
    case 2:
        Globals::inst.StartDSClient(NT_DEFAULT_PORT4);
        m_logs->info("NT", "Requested connect to DS");
        break;
    default:
        break;
    }

    Globals::inst.Disconnect();
}

void SettingsManager::addRecentFile(QFile &file)
{
    QStringList recentFiles = Settings::RecentFiles;

    QString fileName = file.fileName();
    int index = recentFiles.indexOf(fileName);

    if (index != -1) {
        recentFiles.move(index, 0);
    } else {
        recentFiles.prepend(fileName);
    }

    if (recentFiles.length() > 5) {
        recentFiles.removeLast();
    }

    Settings::RecentFiles = recentFiles;

    emit recentFilesChanged();
}

bool SettingsManager::loadRecent() const
{
    return Settings::LoadRecent;
}

void SettingsManager::setLoadRecent(bool newLoadRecent)
{
    Settings::LoadRecent = newLoadRecent;
    emit loadRecentChanged();
}

QStringList SettingsManager::recentFiles() const
{
    return Settings::RecentFiles;
}

void SettingsManager::setRecentFiles(const QStringList &newRecentFiles)
{
    Settings::RecentFiles = newRecentFiles;
    emit recentFilesChanged();
}

QString SettingsManager::theme() const
{
    return Settings::Theme;
}

void SettingsManager::setTheme(const QString &newTheme)
{
    Settings::Theme = newTheme;
    emit themeChanged();
}

QString SettingsManager::accent() const
{
    return Settings::Accent;
}

void SettingsManager::setAccent(const QString &newAccent)
{
    Settings::Accent = newAccent;
    emit accentChanged();
}

QString SettingsManager::ip() const
{
    return Settings::IP;
}

void SettingsManager::setIp(const QString &newIp)
{
    Settings::IP = newIp;
    emit ipChanged();
    reconnectServer();
}

int SettingsManager::team() const
{
    return Settings::TeamNumber;
}

void SettingsManager::setTeam(int newTeam)
{
    Settings::TeamNumber = newTeam;
    emit teamChanged();
    reconnectServer();
}

int SettingsManager::mode() const
{
    return Settings::ConnMode;
}

void SettingsManager::setMode(int newMode)
{
    Settings::ConnMode = newMode;
    emit modeChanged();
    reconnectServer();
}

double SettingsManager::scale() const
{
    return Settings::Scale;
}

void SettingsManager::setScale(double newScale)
{
    Settings::Scale = newScale;
    emit scaleChanged();
}

bool SettingsManager::resizeToDS() const
{
    return Settings::ResizeToDS;
}

void SettingsManager::setResizeToDS(bool newResizeToDS)
{
    Settings::ResizeToDS = newResizeToDS;
    emit resizeToDSChanged();
}

int SettingsManager::logLevel() const
{
    return Settings::LogLevel.value().toInt();
}

void SettingsManager::setLogLevel(int newLogLevel)
{
    Settings::LogLevel.setValue(newLogLevel);
    emit logLevelChanged();
}
