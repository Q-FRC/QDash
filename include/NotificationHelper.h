#ifndef NOTIFICATIONHELPER_H
#define NOTIFICATIONHELPER_H

#include <QObject>
#include <QQmlEngine>

class NotificationHelper : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged FINAL)
    Q_PROPERTY(QString level READ level WRITE setLevel NOTIFY levelChanged FINAL)
    Q_PROPERTY(int displayTime READ displayTime WRITE setDisplayTime NOTIFY displayTimeChanged FINAL)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged FINAL)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged FINAL)

public:
    explicit NotificationHelper(QObject *parent = nullptr);

    void fromJson(const QJsonDocument &doc);

    QString title() const;
    void setTitle(const QString &newTitle);

    QString text() const;
    void setText(const QString &newText);

    QString level() const;
    void setLevel(const QString &newLevel);

    int displayTime() const;
    void setDisplayTime(int newDisplayTime);

    int width() const;
    void setWidth(int newWidth);

    int height() const;
    void setHeight(int newHeight);

signals:
    void titleChanged();

    void textChanged();

    void levelChanged();

    void displayTimeChanged();

    void widthChanged();

    void heightChanged();

    void ready();

private:
    QString m_title;
    QString m_text;
    QString m_level;
    int m_displayTime;
    int m_width;
    int m_height;
};

#endif // NOTIFICATIONHELPER_H
