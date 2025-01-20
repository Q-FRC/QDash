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
    Q_PROPERTY(QString accent READ accent WRITE setAccent NOTIFY accentChanged FINAL)

    Q_PROPERTY(int team READ team WRITE setTeam NOTIFY teamChanged FINAL)
    Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged FINAL)

    Q_PROPERTY(double scale READ scale WRITE setScale NOTIFY scaleChanged FINAL)
    Q_PROPERTY(bool resizeToDS READ resizeToDS WRITE setResizeToDS NOTIFY resizeToDSChanged FINAL)

public:
    explicit SettingsManager(QObject *parent = nullptr);

    // NT
    QString ip() const;
    void setIp(const QString &newIp);

    int team() const;
    void setTeam(int newTeam);

    int mode() const;
    void setMode(int newMode);

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

    double scale() const;
    void setScale(double newScale);

    bool resizeToDS() const;
    void setResizeToDS(bool newResizeToDS);

signals:
    void recentFilesChanged();
    void loadRecentChanged();
    void themeChanged();
    void accentChanged();

    void ipChanged();
    void teamChanged();
    void modeChanged();
    void scaleChanged();
    void resizeToDSChanged();
};

#endif // SETTINGSMANAGER_H
