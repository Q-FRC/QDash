#include "Clipboard.h"
#include <QGuiApplication>
#include <QClipboard>

Clipboard::Clipboard(QObject *parent)
    : QObject{parent}
{}

void Clipboard::copy(const QString &text)
{
    qApp->clipboard()->setText(text);
}
