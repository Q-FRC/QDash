#include "CameraListModel.h"
#include "Globals.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

CameraListModel::CameraListModel(TopicStore &store, QObject *parent)
    : QAbstractListModel(parent)
    , m_store(&store)
{}

int CameraListModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant CameraListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Camera c = m_data[index.row()];

    switch (role) {
    case NAME:
        return c.name;
    case URLS:
        return QVariant::fromValue(c.urls);
    case SOURCE:
        return c.source;
    default:
        break;
    }

    return QVariant();
}

bool CameraListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // if (data(index, role) != value) {
    //     Tab &t = m_data[index.row()];
    //     switch (role) {
    //     case TITLE:
    //         t.title = value.toString();
    //     case ROWS:
    //         t.rows = value.toInt();
    //     case COLS:
    //         t.cols = value.toInt();
    //     case WIDGETS:
    //         t.model = value.value<TabWidgetsModel *>();
    //     default:
    //         break;
    //     }
    //     emit dataChanged(index, index, {role});
    //     return true;
    // }
    return false;
}

Qt::ItemFlags CameraListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void CameraListModel::add(std::shared_ptr<nt::NetworkTable> table)
{
    Camera camera{};

    camera.name = QString::fromStdString(table->GetEntry("description").GetString(""));
    camera.source = QString::fromStdString(table->GetEntry("source").GetString(""));

    QString namePath = QString::fromStdString(std::string{table->GetPath()} + "/description");
    QString sourcePath = QString::fromStdString(std::string{table->GetPath()} + "/source");
    QString streamsPath = QString::fromStdString(std::string{table->GetPath()} + "/streams");

    nt::NetworkTableEntry desc = Globals::inst.GetEntry(namePath.toStdString());
    nt::NetworkTableEntry src = Globals::inst.GetEntry(sourcePath.toStdString());
    nt::NetworkTableEntry str = Globals::inst.GetEntry(streamsPath.toStdString());

    camera.name = QString::fromStdString(desc.GetString("invalid"));
    camera.source = QString::fromStdString(src.GetString("invalid"));

    auto urls = TopicStore::toVariant(str.GetValue()).toList();

    camera.urls = {};
    for (const QVariant &url : urls) {
        static QRegularExpression re("^(mjpe?g|ip|usb):");
        QString newStream = url.toUrl().toString();
        newStream.replace(re, "");
        newStream.replace("/?action=stream", "/stream.mjpg?");

        camera.urls.append(QUrl(newStream));
    }

    bool descriptionDone = (camera.name != "invalid"),
        sourceDone = (camera.source != "invalid"),
        streamsDone = (!camera.urls.empty());

    if (sourceDone && descriptionDone && streamsDone) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_data << camera;
        endInsertRows();
    } else {
        m_store->subscribe(namePath);
        m_store->subscribe(sourcePath);
        m_store->subscribe(streamsPath);

        QMetaObject::Connection *conn = new QMetaObject::Connection;

        *conn = connect(m_store, &TopicStore::topicUpdate, this, [=, this](QString topic, QVariant value) mutable {
            if (!descriptionDone && topic == namePath) {
                descriptionDone = true;

                camera.name = value.toString();

                if (camera.name.isEmpty()) {
                    std::string path{table->GetPath()};
                    QString qpath = QString::fromStdString(path);
                    camera.name = qpath.split("/").last();
                }
            } else if (!sourceDone && topic == sourcePath) {
                sourceDone = true;

                camera.source = value.toString();
            } else if (!streamsDone && topic == streamsPath) {
                streamsDone = true;

                QStringList streams = value.toStringList();

                for (const QString &stream : streams) {
                    static QRegularExpression re("^(mjpe?g|ip|usb):");
                    QString newStream = stream;
                    newStream.replace(re, "");
                    newStream.replace("/?action=stream", "/stream.mjpg?");

                    camera.urls.append(QUrl(newStream));
                }
            }

            if ((topic == streamsPath || topic == sourcePath || topic == namePath)
                && sourceDone && descriptionDone && streamsDone) {

                beginInsertRows(QModelIndex(), rowCount(), rowCount());
                m_data << camera;
                endInsertRows();

                m_store->unsubscribe(namePath);
                m_store->unsubscribe(sourcePath);
                m_store->unsubscribe(streamsPath);

                disconnect(*conn);
                delete conn;
            }
        });
    }
}

void CameraListModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

bool CameraListModel::remove(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    m_data.remove(row);
    endRemoveRows();

    return true;
}

QHash<int, QByteArray> CameraListModel::roleNames() const
{
    QHash<int,QByteArray> rez;
    rez[NAME] = "name";
    rez[URLS] = "urls";
    rez[SOURCE] = "source";

    return rez;
}
