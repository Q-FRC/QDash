#include "SettingsManager.h"
#include "Constants.h"
#include "Globals.h"

SettingsManager::SettingsManager(QObject *parent)
    : QObject{parent}
{}

void SettingsManager::reconnectServer() {
    std::string server = Globals::server.server;
    bool isTeamNumber = Globals::server.teamNumber;
    int port = Globals::server.port;
    QString switchTopic = Globals::server.switchTopic;

    if (server.empty()) return;

    if (isTeamNumber) {
        int team;
        try {
            team = std::stoi(server);
        } catch (std::invalid_argument const &) {
            return;
        }

        Globals::inst.SetServerTeam(team, port);
    } else {
        Globals::inst.SetServer(server.c_str(), port);
    }

    Globals::inst.Disconnect();

    QString serverTopic = Globals::server.switchTopic;

    if (serverTopic != switchTopic) {
        emit switchTopicChanged();
    }
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

int SettingsManager::getPort() const
{
    return Globals::server.port;
}

void SettingsManager::setPort(int newPort)
{
    Globals::server.port = newPort;
    emit portChanged();
    reconnectServer();
}

QString SettingsManager::switchTopic() const
{
    return Globals::server.switchTopic;
}

void SettingsManager::setSwitchTopic(const QString &newTopic)
{
    Globals::server.switchTopic = newTopic;
    emit switchTopicChanged();
    reconnectServer();
}

QString SettingsManager::ip() const
{
    return QString::fromStdString(Globals::server.server);
}

void SettingsManager::setIp(const QString &newIp)
{
    Globals::server.server = newIp.toStdString();
    emit ipChanged();
    reconnectServer();
}

bool SettingsManager::useTeam() const
{
    return Globals::server.teamNumber;
}

void SettingsManager::setUseTeam(bool newUseTeam)
{
    Globals::server.teamNumber = newUseTeam;
    emit useTeamChanged();
    reconnectServer();
}
