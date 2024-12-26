#ifndef PLATFORMHELPER_H
#define PLATFORMHELPER_H

#include <QObject>
#include <QQmlEngine>

class PlatformHelper : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    PlatformHelper(QObject *parent = nullptr);

    Q_INVOKABLE void copy(const QString &text);
    Q_INVOKABLE QString baseName(const QString &file);

signals:
};

#endif // PLATFORMHELPER_H
