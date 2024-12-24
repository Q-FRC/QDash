#include "TabListModel.h"
#include "Globals.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

TabListModel::TabListModel(SettingsManager *settings, QObject *parent)
    : QAbstractListModel(parent)
    , m_settings(settings)
{}

int TabListModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant TabListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Tab t = m_data[index.row()];

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

void TabListModel::add(Tab t)
{
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

    t.model = nullptr;

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
    name.replace("file://", "");
    QFile file(name);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Failed to open file" << name << "for writing.";
        return;
    }

    m_settings->addRecentFile(file);

    QTextStream stream(&file);
    stream << saveObject().toJson();
    file.close();
}

QJsonDocument TabListModel::saveObject() const
{
    QJsonObject doc;
    doc.insert("mode", m_settings->mode());
    doc.insert("team", m_settings->team());
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

void TabListModel::loadObject(const QJsonDocument &doc)
{
    QJsonObject ob = doc.object();

    Globals::server.ip = ob.value("ip").toString().toStdString();
    Globals::server.team = ob.value("team").toInteger();
    Globals::server.mode = ob.value("mode").toInteger();

    m_settings->reconnectServer();

    QJsonArray arr = ob.value("tabs").toArray();

    for (const QJsonValueRef ref : arr) {
        QJsonObject obj = ref.toObject();

        Tab t;

        t.title = obj.value("title").toString();
        t.rows = obj.value("rows").toInt();
        t.cols = obj.value("cols").toInt();
        t.model = TabWidgetsModel::loadObject(this, obj.value("widgets").toArray());
        t.model->setCols(t.cols);
        t.model->setRows(t.rows);

        add(t);
    }
}

void TabListModel::load(const QString &filename)
{
    QString name = filename;
    name.replace("file://", "");

    QFile file(name);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    m_settings->addRecentFile(file);

    QTextStream stream(&file);
    QByteArray data = stream.readAll().toUtf8();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    clear();

    loadObject(doc);
    file.close();
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
    for (size_t i = 0; i < rowCount(); ++i) {
        Tab t = m_data.at(i);
        if (t.title == tab) {
            m_selectedTab = i;
            emit selectedTabChanged();
            return;
        }
    }

    qWarning() << "Selected tab" << tab << "does not exist.";
}

QHash<int, QByteArray> TabListModel::roleNames() const
{
    QHash<int,QByteArray> rez;
    rez[TITLE] = "title";
    rez[ROWS] = "rows";
    rez[COLS] = "cols";
    rez[WIDGETS] = "widgets";

    return rez;
}
