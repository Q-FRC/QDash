#include "Models/GenericMapModel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QRect>
#include <qcolor.h>

GenericMapModel::GenericMapModel(QObject *parent)
    : QAbstractListModel(parent)
{}

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

// bool GenericMapModel::setData(const QModelIndex &index, const QVariant &value, int role)
// {
//     // if (data(index, role) != value) {
//     //     Widget &w = m_data[index.row()];

//     //     switch (role) {
//     //     case TITLE:
//     //         w.title = value.toString();
//     //         break;
//     //     case TYPE:
//     //         w.type = value.toString();
//     //         break;
//     //     case TOPIC:
//     //         w.topic = value.toString();
//     //         break;
//     //     case COL:
//     //         w.col = value.toInt();
//     //         break;
//     //     case ROW:
//     //         w.row = value.toInt();
//     //         break;
//     //     case COLSPAN:
//     //         w.colSpan = value.toInt();
//     //         break;
//     //     case ROWSPAN:
//     //         w.rowSpan = value.toInt();
//     //         break;
//     //     case PROPERTIES:
//     //         w.properties = value.toMap();
//     //         break;
//     //     }

//     //     emit dataChanged(index, index, {role});
//     //     return true;
//     // }
//     return false;
// }

QHash<int, QByteArray> GenericMapModel::roleNames() const
{
    QHash<int,QByteArray> rez;
    rez[KEY] = "key";
    rez[VALUE] = "value";

    return rez;
}
