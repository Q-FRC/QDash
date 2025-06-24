#include "NotificationHelper.h"

#include <QJsonDocument>
#include <QJsonObject>

NotificationHelper::NotificationHelper(QObject *parent)
    : QObject{parent}
{}

void NotificationHelper::fromJson(const QJsonDocument &doc)
{
    QJsonObject obj = doc.object();

    setTitle(obj.value("title").toString("Notification"));
    setText(obj.value("description").toString("Text"));
    setLevel(obj.value("level").toString("Information"));

    setDisplayTime(obj.value("displayTime").toInt(3000));
    setWidth(obj.value("width").toInt(350));
    setHeight(obj.value("height").toInt(-1));

    emit ready();
}

QString NotificationHelper::title() const
{
    return m_title;
}

void NotificationHelper::setTitle(const QString &newTitle)
{
    if (m_title == newTitle)
        return;
    m_title = newTitle;
    emit titleChanged();
}

QString NotificationHelper::text() const
{
    return m_text;
}

void NotificationHelper::setText(const QString &newText)
{
    if (m_text == newText)
        return;
    m_text = newText;
    emit textChanged();
}

QString NotificationHelper::level() const
{
    return m_level;
}

void NotificationHelper::setLevel(const QString &newLevel)
{
    if (m_level == newLevel)
        return;
    m_level = newLevel;
    emit levelChanged();
}

int NotificationHelper::displayTime() const
{
    return m_displayTime;
}

void NotificationHelper::setDisplayTime(int newDisplayTime)
{
    if (m_displayTime == newDisplayTime)
        return;
    m_displayTime = newDisplayTime;
    emit displayTimeChanged();
}

int NotificationHelper::width() const
{
    return m_width;
}

void NotificationHelper::setWidth(int newWidth)
{
    if (m_width == newWidth)
        return;
    m_width = newWidth;
    emit widthChanged();
}

int NotificationHelper::height() const
{
    return m_height;
}

void NotificationHelper::setHeight(int newHeight)
{
    if (m_height == newHeight)
        return;
    m_height = newHeight;
    emit heightChanged();
}
