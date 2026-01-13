// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QJsonArray>
#include <QJsonObject>
#include <QRect>
#include <qcolor.h>
#include "Models/GenericMapModel.h"

GenericMapModel::GenericMapModel(QObject *parent) : QAbstractListModel(parent) {}

int GenericMapModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant GenericMapModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Data d = m_data[index.row()];

    switch (role) {
    case KEY:
        return d.key;
    case VALUE:
        return d.value;
    default:
        break;
    }

    return QVariant();
}

void GenericMapModel::add(QString key, QString value)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    Data d{key, value};
    m_data << d;
    endInsertRows();
}

int GenericMapModel::count() {
    return m_data.count();
}

QHash<int, QByteArray> GenericMapModel::roleNames() const
{
    QHash<int, QByteArray> rez;
    rez[KEY] = "key";
    rez[VALUE] = "value";

    return rez;
}
