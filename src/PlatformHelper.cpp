#include "PlatformHelper.h"
#include <QGuiApplication>
#include <QClipboard>
#include <QFileInfo>

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
