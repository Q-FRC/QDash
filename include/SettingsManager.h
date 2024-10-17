#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QFile>
#include <QObject>
#include <QQmlEngine>

class SettingsManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool loadRecent READ loadRecent WRITE setLoadRecent NOTIFY loadRecentChanged FINAL)
    Q_PROPERTY(QStringList recentFiles READ recentFiles WRITE setRecentFiles NOTIFY recentFilesChanged FINAL)
    Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged FINAL)

    Q_PROPERTY(bool useTeam READ useTeam WRITE setUseTeam NOTIFY useTeamChanged FINAL)
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged FINAL)
    Q_PROPERTY(int port READ getPort WRITE setPort NOTIFY portChanged FINAL)
    Q_PROPERTY(QString switchTopic READ switchTopic WRITE setSwitchTopic NOTIFY switchTopicChanged FINAL)
public:
    explicit SettingsManager(QObject *parent = nullptr);

    // NT
    bool useTeam() const;
    void setUseTeam(bool newUseTeam);

    QString ip() const;
    void setIp(const QString &newIp);

    int getPort() const;
    void setPort(int newPort);

    QString switchTopic() const;
    void setSwitchTopic(const QString &newTopic);

    // other
    bool loadRecent() const;
    void setLoadRecent(bool newLoadRecent);

    QStringList recentFiles() const;
    void setRecentFiles(const QStringList &newRecentFiles);

    void addRecentFile(QFile &file);

    void reconnectServer();

    QString theme() const;
    void setTheme(const QString &newTheme);

    QString accent() const;
    void setAccent(const QString &newAccent);

signals:
    void useTeamChanged();
    void ipChanged();
    void portChanged();
    void switchTopicChanged();

    void recentFilesChanged();
    void loadRecentChanged();
    void themeChanged();
    void accentChanged();
private:
    Q_PROPERTY(QString accent READ accent WRITE setAccent NOTIFY accentChanged FINAL)
};

#endif // SETTINGSMANAGER_H
