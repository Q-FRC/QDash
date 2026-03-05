// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TabListModel.h"

#include "TabWidgetsModel.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QSaveFile>

TabListModel::TabListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int TabListModel::rowCount(const QModelIndex &_) const
{
    return m_data.count();
}

QVariant TabListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const Tab &t = m_data.at(index.row());

    switch (role) {
    case TITLE:
        return t.title;
    case ROWS:
        return t.rows;
    case COLS:
        return t.cols;
    case WIDGETS:
        return QVariant::fromValue(t.model);
    default:
        break;
    }

    return QVariant();
}

bool TabListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        Tab &t = m_data[index.row()];
        switch (role) {
        case TITLE:
            t.title = value.toString();
            break;
        case ROWS:
            t.rows = value.toInt();
            break;
        case COLS:
            t.cols = value.toInt();
            break;
        case WIDGETS:
            t.model = value.value<TabWidgetsModel *>();
            break;
        default:
            break;
        }
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags TabListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void TabListModel::reset(const QList<Tab> &data) {
    beginResetModel();
    m_data = data;
    endResetModel();
}

void TabListModel::add(const Tab &t) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data << t;
    endInsertRows();
}

void TabListModel::add(QString title)
{
    Tab t;
    t.title = title;
    t.rows = 3;
    t.cols = 5;

    t.model = new TabWidgetsModel(this);
    t.model->setRows(t.rows);
    t.model->setCols(t.cols);

    add(t);
}

bool TabListModel::remove(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    m_data.remove(row);
    endRemoveRows();

    return true;
}

TabListModel *TabListModel::loadObject(QObject *parent, const QJsonArray &arr)
{
    TabListModel *model = new TabListModel(parent);

    QList<Tab> tabs;
    tabs.reserve(arr.size());

    for (const QJsonValueConstRef ref : std::as_const(arr)) {
        QJsonObject obj = ref.toObject();

        Tab t;

        t.title = obj.value("title").toString();
        t.rows = obj.value("rows").toInt();
        t.cols = obj.value("cols").toInt();
        t.model = TabWidgetsModel::loadObject(model, obj.value("widgets").toArray());
        t.model->setCols(t.cols);
        t.model->setRows(t.rows);

        tabs << t;
    }

    model->reset(tabs);
    return model;
}

QJsonObject TabListModel::saveObject() const
{
    QJsonObject obj;
    QJsonArray arr;

    for (const Tab &t : m_data) {
        QJsonObject tab;

        tab.insert("title", t.title);
        tab.insert("rows", t.rows);
        tab.insert("cols", t.cols);
        tab.insert("widgets", t.model->saveObject());

        arr.append(tab);
    }

    obj.insert("tabs", arr);

    return obj;
}



void TabListModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

int TabListModel::selectedTab() const
{
    return m_selectedTab;
}

void TabListModel::selectTab(const QString &tab)
{
    for (int i = 0; i < rowCount(); ++i) {
        Tab t = m_data.at(i);
        if (t.title == tab) {
            m_selectedTab = i;
            emit selectedTabChanged();
            return;
        }
    }
}

QHash<int, QByteArray> TabListModel::roleNames() const
{
    QHash<int, QByteArray> rez;
    rez[TITLE] = "title";
    rez[ROWS] = "rows";
    rez[COLS] = "cols";
    rez[WIDGETS] = "widgets";

    return rez;
}
