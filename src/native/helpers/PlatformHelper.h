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

    Q_INVOKABLE double screenWidth();
    Q_INVOKABLE double screenHeight();

    Q_INVOKABLE bool isMac();
signals:
};

#endif // PLATFORMHELPER_H
