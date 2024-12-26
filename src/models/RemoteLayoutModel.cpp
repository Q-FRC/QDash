#include "RemoteLayoutModel.h"
#include "Globals.h"

#include <QNetworkReply>

RemoteLayoutModel::RemoteLayoutModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int RemoteLayoutModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant RemoteLayoutModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    RemoteLayout rl = m_data[index.row()];

    switch (role) {
    case URL:
        return rl.url;
    case NAME:
        return rl.name;
    case IDX:
        return index.row();
    default:
        break;
    }

    return QVariant();
}

void RemoteLayoutModel::add(const QUrl &url, const QString &name)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    RemoteLayout rl;
    rl.url = url;
    rl.name = name;

    m_data << rl;

    endInsertRows();
}

bool RemoteLayoutModel::load()
{
    clear();
    auto conns = Globals::inst.GetConnections();
    if (conns.empty()) return false;

    nt::ConnectionInfo info = conns.at(0);
    QString ip = QString::fromStdString(info.remote_ip);

    // assume port 5800
    QUrl url = "http://" + ip + ":5800/?format=json";

    QNetworkRequest req(url);
    QNetworkReply *reply = m_manager.get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply, ip] {
        QByteArray response = reply->readAll();

        QJsonDocument doc = QJsonDocument::fromJson(response);

        QJsonObject obj = doc.object();
        QJsonArray files = obj.value("files").toArray();

        for (const QJsonValueRef ref : files) {
            QJsonObject obj = ref.toObject();

            QString name = obj.value("name").toString();

            if (name.endsWith(".json")) {
                QUrl url = "http://" + ip + ":5800/" + name;

                add(url, name);
            }
        }

        emit listReady();
    });

    return true;
}

void RemoteLayoutModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

void RemoteLayoutModel::download(const QUrl &url, const QString &filename)
{
    QNetworkRequest req(url);
    QNetworkReply *reply = m_manager.get(req);
    QString name = filename;
    name.replace("file://", "");

    connect(reply, &QNetworkReply::finished, this, [this, reply, name] {
        QFile file(name);

        QByteArray content = reply->readAll();

        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            return;
        }

        file.write(content);

        file.close();

        emit fileOpened(name);
    });
}

QString RemoteLayoutModel::name(int index)
{
    return m_data[index].name;
}

QUrl RemoteLayoutModel::url(int index)
{
    return m_data[index].url;
}

QHash<int, QByteArray> RemoteLayoutModel::roleNames() const
{
    QHash<int,QByteArray> rez;
    rez[URL] = "url";
    rez[NAME] = "name";
    rez[IDX] = "idx";

    return rez;
}
