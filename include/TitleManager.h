#ifndef TITLEMANAGER_H
#define TITLEMANAGER_H

#include "BuildConfig.h"
#include <QObject>
#include <QQmlEngine>

class TitleManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TitleManager(QObject *parent = nullptr);

    QString title() const;
    void setTitle(const QString &newTitle);
    void resetTitle();

private:
    QString m_title = BuildConfig.APP_NAME + " - Not Connected";
    Q_PROPERTY(QString title READ title WRITE setTitle RESET resetTitle NOTIFY titleChanged FINAL)

signals:
    void titleChanged();
};

#endif // TITLEMANAGER_H
