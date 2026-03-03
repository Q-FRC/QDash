// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TabListModel.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QSaveFile>

TabListModel::TabListModel(Logger *logs, SettingsManager *settings, QObject *parent)
    : QAbstractListModel(parent), m_settings(settings), m_logs(logs)
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

void TabListModel::add(Tab t) {
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

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data << t;
    endInsertRows();
}

bool TabListModel::remove(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    m_data.remove(row);
    endRemoveRows();

    return true;
}

void TabListModel::save(const QString &filename)
{
    QString name = filename;
#ifdef Q_OS_WINDOWS
    name.replace("file:///", "");
#else
    name.replace("file://", "");
#endif

    QSaveFile file(name);

    QByteArray data = saveObject().toJson(QJsonDocument::Compact);

    if (file.write(data) == -1) {
        m_logs->critical("Layout", "Failed to write layout to " + name);
        file.cancelWriting();
        return;
    }

    if (!file.commit()) {
        m_logs->critical("Layout", "Failed to commit layout file " + name);
        return;
    }

    m_settings->addRecentFile(file.fileName());

}

QJsonDocument TabListModel::saveObject() const
{
    QJsonObject doc;
    doc.insert("mode", m_settings->connMode());
    doc.insert("team", m_settings->teamNumber());
    doc.insert("ip", m_settings->ip());

    QJsonArray arr;

    for (const Tab &t : m_data) {
        QJsonObject obj;

        obj.insert("title", t.title);
        obj.insert("rows", t.rows);
        obj.insert("cols", t.cols);
        obj.insert("widgets", t.model->saveObject());

        arr.append(obj);
    }

    doc.insert("tabs", arr);

    return QJsonDocument(doc);
}

void TabListModel::load(const QString &filename)
{
    QString name = filename;
#ifdef Q_OS_WINDOWS
    name.replace("file:///", "");
#else
    name.replace("file://", "");
#endif

    QFile file(name);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    m_settings->addRecentFile(file);

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    file.close();

    QJsonObject ob = doc.object();

    QJsonArray arr = ob.value("tabs").toArray();

    QList<Tab> tabs;
    tabs.reserve(arr.size());

    for (const QJsonValueRef ref : arr) {
        QJsonObject obj = ref.toObject();

        Tab t;

        t.title = obj.value("title").toString();
        t.rows = obj.value("rows").toInt();
        t.cols = obj.value("cols").toInt();
        t.model = TabWidgetsModel::loadObject(this, obj.value("widgets").toArray());
        t.model->setCols(t.cols);
        t.model->setRows(t.rows);

        tabs << t;
    }

    m_settings->reconnect();
    beginResetModel();
    m_data = tabs;
    endResetModel();
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

    m_logs->warn("Layout", "Failed to select nonexistent " + tab + ".");
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
