#pragma once
#include <QString>
#include <QObject>
#include <QtQmlIntegration/QtQmlIntegration>

class Config : public QObject {
    Q_OBJECT
    QML_ELEMENT
public:
    Config();
    QString APP_NAME;
    QString ORG_NAME;
    QString GIT_REPO;

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

    /// tag-commit
    QString versionString() const;

    Q_INVOKABLE QString buildInfo() const;
};

extern Config BuildConfig;

Q_DECLARE_METATYPE(Config);
