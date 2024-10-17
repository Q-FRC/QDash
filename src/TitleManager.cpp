#include "TitleManager.h"

TitleManager::TitleManager(QObject *parent)
    : QObject{parent}
{}

QString TitleManager::title() const
{
    return m_title;
}

void TitleManager::setTitle(const QString &newTitle)
{
    m_title = BuildConfig.APP_NAME + " - " + newTitle;
    emit titleChanged();
}

void TitleManager::resetTitle()
{
    setTitle("Not Connected");
}
