// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Models/AccentsListModel.h"

#include "buildconfig/BuildConfig.h"

#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <qguiapplication.h>

AccentsListModel::AccentsListModel(QObject *parent) : QAbstractListModel{parent} {}

int AccentsListModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant AccentsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Accent a = m_data[index.row()];

    switch (role) {
    case NAME:
        return a.name;
    case ACCENT:
        return a.accent;
    case LIGHT:
        return a.light;
    case QML:
        return a.qml;
    case IDX:
        return index.row();
    default:
        break;
    }

    return QVariant();
}

void AccentsListModel::loadObject(const QJsonDocument &doc)
{
    QJsonArray arr = doc.array();

    for (const QJsonValueConstRef ref : std::as_const(arr)) {
        QJsonObject obj = ref.toObject();

        Accent a;

        a.name = obj.value("name").toString();
        a.accent = obj.value("accent").toString();
        a.light = obj.value("light").toString();
        a.qml = obj.value("qml").toString();

        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_data << a;
        endInsertRows();
    }
}

void AccentsListModel::load()
{
    static QString name = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QDir dir(name);
    dir.mkpath(BuildConfig.ORGANIZATION_NAME);
    dir.cd(BuildConfig.ORGANIZATION_NAME);

    QFile file(":/accents.json");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Failed to load bundled accents!";
        std::exit(1);
    }

    QTextStream stream(&file);
    QByteArray data = stream.readAll().toUtf8();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    beginResetModel();
    m_data.clear();
    endResetModel();

    loadObject(doc);
    file.close();
}

QString AccentsListModel::accent(const QString &name)
{
    for (const Accent &a : std::as_const(m_data)) {
        if (name == a.name) {
            return a.accent;
        }
    }

    return "";
}

QString AccentsListModel::light(const QString &name)
{
    for (const Accent &a : std::as_const(m_data)) {
        if (name == a.name) {
            return a.light;
        }
    }

    return "";
}

QString AccentsListModel::qml(const QString &name)
{
    for (const Accent &a : std::as_const(m_data)) {
        if (name == a.name) {
            return a.qml;
        }
    }

    return "";
}

QStringList AccentsListModel::names() const
{
    QStringList list;
    for (const Accent &a : m_data) {
        // convert to title case
        QString name = a.name;
        QChar first = name.at(0).toUpper();

        list.append(first + name.last(name.size() - 1));
    }

    return list;
}

QHash<int, QByteArray> AccentsListModel::roleNames() const
{
    QHash<int, QByteArray> rez;
    rez[NAME] = "name";
    rez[ACCENT] = "accent";
    rez[LIGHT] = "light";
    rez[QML] = "qml";
    rez[IDX] = "idx";

    return rez;
}
