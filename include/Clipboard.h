#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QObject>
#include <QQmlEngine>

class Clipboard : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    Clipboard(QObject *parent = nullptr);

    Q_INVOKABLE void copy(const QString &text);

signals:
};

#endif // CLIPBOARD_H
