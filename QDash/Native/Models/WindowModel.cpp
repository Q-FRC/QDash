#include "WindowModel.h"

#include "TabListModel.h"

#include <QJsonObject>
#include <QSaveFile>

WindowModel::WindowModel(Logger *logs, SettingsManager *settings, QObject *parent)
    : m_logs(logs), m_settings(settings), QAbstractListModel(parent)
{
}

int WindowModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant WindowModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant{};

    const Window &w = m_data.at(index.row());

    switch (role) {
    case TITLE:
        return w.title;
    case GEOMETRY:
        return w.geometry;
    case TABS:
        return QVariant::fromValue(w.model);
    default:
        break;
    }

    return QVariant{};
}

bool WindowModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        Window &w = m_data[index.row()];
        switch (role) {
        case TITLE:
            w.title = value.toString();
            break;
        case GEOMETRY:
            w.geometry = value.toRect();
            break;
        case TABS:
            w.model = value.value<TabListModel *>();
            break;
        default:
            break;
        }
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags WindowModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void WindowModel::add(const QString &title)
{
    Window w;
    w.title = title;
    w.model = new TabListModel(this);

    add(w);
}

void WindowModel::add(const Window &w)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data << w;
    endInsertRows();
}

bool WindowModel::remove(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    m_data.remove(row);
    endRemoveRows();

    return true;
}

void WindowModel::save(const QString &filename)
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

QJsonDocument WindowModel::saveObject() const {
    QJsonObject doc;
}

void WindowModel::load(const QString &filename)
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

    // TODOO TOOOTOTOTOTOTOOTODODODOOTODOTDOTDOO

    const QJsonArray &arr = doc.array();

    // TODO(crueter): If there is a tab value or ip/team in the doc, use legacy method

    QList<Window> windows;
    windows.reserve(arr.size());

    for (const QJsonValueConstRef &ref : std::as_const(arr)) {
        QJsonObject obj = ref.toObject();

        Window w;

        w.title = obj.value("title").toString();

        qsizetype x, y, wi, he;
        x = obj.value("x").toInt(-1);
        y = obj.value("y").toInt(-1);
        wi = obj.value("width").toInt(-1);
        he = obj.value("height").toInt(-1);

        w.geometry = QRect(x, y, wi, he);

        w.model = TabListModel::loadObject(this, obj.value("tabs").toArray());

        windows << w;
    }

    m_settings->reconnect();

    beginResetModel();
    m_data = windows;
    endResetModel();
}

void WindowModel::clear() {}

QHash<int, QByteArray> WindowModel::roleNames() const {}
