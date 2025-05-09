#include "PlatformHelper.h"
#include <QClipboard>
#include <QFileInfo>
#include <QGuiApplication>
#include <QScreen>

PlatformHelper::PlatformHelper(QObject *parent)
    : QObject{parent}
{}

void PlatformHelper::copy(const QString &text)
{
    qApp->clipboard()->setText(text);
}

QString PlatformHelper::baseName(const QString &file)
{
    QFile f(file);
    QFileInfo info(f);
    return info.baseName();
}

double PlatformHelper::screenWidth()
{
    return qApp->primaryScreen()->availableSize().width();
}

double PlatformHelper::screenHeight()
{
    return qApp->primaryScreen()->availableSize().height();
}

bool PlatformHelper::isMac()
{
#ifdef Q_OS_APPLE
    return true;
#else
    return false;
#endif
}
