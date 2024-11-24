#include "MapModel.h"

MapModel::MapModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

QVariant MapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
    case 0:
        return "Value";
    case 1:
        return m_valueName;
    default:
        return "";
    }
}

int MapModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_data.count();
}

int MapModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant MapModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Data d = m_data[index.row()];
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return d.key;
        case 1:
            return d.value;
        default:
            return QVariant();
        }
    }

    switch (role) {
    case KEY:
        return d.key;
    case VALUE:
        return d.value;
    default:
        return QVariant();
    }
}

bool MapModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // if (data(index, role) != value) {
        switch (role) {
        case Qt::DisplayRole:
            if (index.column() == 0) {
                m_data[index.row()].key = value.toString();
            } else {
                m_data[index.row()].value = value.toString();
            }
            break;
        case KEY:
            m_data[index.row()].key = value.toString();
            break;
        case VALUE:
            m_data[index.row()].value = value.toString();
            break;
        default:
            break;
        }
        emit dataChanged(index, index, {role});
        return true;
//     }
// return false;
}

Qt::ItemFlags MapModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

void MapModel::add(QString key, QString value)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    Data d{key, value};
    m_data << d;
    endInsertRows();
}

void MapModel::remove(int row)
{
    if (row < 0 || row >= m_data.count()) return;
    beginRemoveRows(QModelIndex(), row, row);
    m_data.remove(row);
    endRemoveRows();
}

QVariantList MapModel::asList()
{
    QVariantList list;

    for (const Data &d : m_data) {
        QVariantMap map;
        map.insert("Value", d.key);
        map.insert(m_valueName, d.value);
        list.append(map);
    }

    return list;

}

void MapModel::toList(const QVariantList &list)
{
    beginResetModel();
    m_data.clear();
    endResetModel();

    for (const QVariant &v : list) {
        QVariantMap m = v.toMap();
        add(m.value("Value").toString(), m.value(m_valueName).toString());
    }
}

QString MapModel::valueName() const
{
    return m_valueName;
}

void MapModel::setValueName(const QString &newValueName)
{
    if (m_valueName == newValueName)
        return;
    m_valueName = newValueName;
    emit valueNameChanged();
}

QHash<int, QByteArray> MapModel::roleNames() const
{
    QHash<int,QByteArray> rez;
    rez[Qt::DisplayRole] = "display";
    rez[KEY] = "key";
    rez[VALUE] = "value";

    return rez;
}
