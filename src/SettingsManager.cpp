#include "SettingsManager.h"
#include "Constants.h"
#include "Globals.h"

SettingsManager::SettingsManager(QObject *parent)
    : QObject{parent}
{}

void SettingsManager::reconnectServer() {
    std::string server = Globals::server.ip;
    int team = Globals::server.team;
    int mode = Globals::server.mode;

    switch (mode) {
    // IP Address
    case 0:
        Globals::inst.SetServer(server.c_str(), NT_DEFAULT_PORT4);
        break;
    // Team Number
    case 1:
        Globals::inst.SetServerTeam(team, NT_DEFAULT_PORT4);
        break;
    // DS
    case 2:
        Globals::inst.StartDSClient(NT_DEFAULT_PORT4);
        break;
    default:
        break;
    }

    Globals::inst.Disconnect();
}

void SettingsManager::addRecentFile(QFile &file) {
    QStringList recentFiles = Settings::RecentFiles.value().toStringList();

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

    Settings::RecentFiles.setValue(recentFiles);

    emit recentFilesChanged();
}

bool SettingsManager::loadRecent() const
{
    return Settings::LoadRecent.value().toBool();
}

void SettingsManager::setLoadRecent(bool newLoadRecent)
{
    Settings::LoadRecent.setValue(newLoadRecent);
    emit loadRecentChanged();
}


QStringList SettingsManager::recentFiles() const
{
    return Settings::RecentFiles.value().toStringList();
}

void SettingsManager::setRecentFiles(const QStringList &newRecentFiles)
{
    Settings::RecentFiles.setValue(newRecentFiles);
    emit recentFilesChanged();
}

QString SettingsManager::theme() const
{
    return Settings::Theme.value().toString();
}

void SettingsManager::setTheme(const QString &newTheme)
{
    Settings::Theme.setValue(newTheme);
    emit themeChanged();
}

QString SettingsManager::accent() const
{
    return Settings::Accent.value().toString();
}

void SettingsManager::setAccent(const QString &newAccent)
{
    Settings::Accent.setValue(newAccent);
    emit accentChanged();
}

QString SettingsManager::ip() const
{
    return QString::fromStdString(Globals::server.ip);
}

void SettingsManager::setIp(const QString &newIp)
{
    Globals::server.ip = newIp.toStdString();
    emit ipChanged();
    reconnectServer();
}


int SettingsManager::team() const
{
    return Globals::server.team;
}

void SettingsManager::setTeam(int newTeam)
{
    Globals::server.team = newTeam;
    emit teamChanged();
    reconnectServer();
}

int SettingsManager::mode() const
{
    return Globals::server.mode;
}

void SettingsManager::setMode(int newMode)
{
    Globals::server.mode = newMode;
    emit modeChanged();
    reconnectServer();
}
