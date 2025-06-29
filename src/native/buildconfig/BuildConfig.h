#pragma once
#include <QString>
#include <QObject>
#include <QQmlEngine>

class Config : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    Config();
    QString APPLICATION_NAME;
    QString ORGANIZATION_NAME;
    QString REPOSITORY;

    /// A short string identifying this build's platform. For example, "lin64" or "win32".
    QString BUILD_PLATFORM;

    /// A string containing the build timestamp
    QString BUILD_DATE;

    /// The git commit hash of this build
    QString GIT_COMMIT;

    /// The git tag of this build
    QString GIT_TAG;

    /// The git refspec of this build
    QString GIT_REFSPEC;

    /// Version Channel
    QString VERSION_CHANNEL;

    Q_INVOKABLE QString buildInfo() const;
};

extern Config BuildConfig;

Q_DECLARE_METATYPE(Config);
