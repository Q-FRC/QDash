#include "models/AccentsListModel.h"

#include <BuildConfig.h>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QClipboard>
#include <qguiapplication.h>

AccentsListModel::AccentsListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_universalMap.insert("Lime", "#A4C400");
    m_universalMap.insert("Green", "#60A917");
    m_universalMap.insert("Emerald", "#008A00");
    m_universalMap.insert("Teal", "#00ABA9");
    m_universalMap.insert("Cyan", "#1BA1E2");
    m_universalMap.insert("Cobalt", "#3E65FF");
    m_universalMap.insert("Indigo", "#6A00FF");
    m_universalMap.insert("Violet", "#AA00FF");
    m_universalMap.insert("Pink", "#F472D0");
    m_universalMap.insert("Magenta", "#D80073");
    m_universalMap.insert("Crimson", "#A20025");
    m_universalMap.insert("Red", "#E51400");
    m_universalMap.insert("Orange", "#FA6800");
    m_universalMap.insert("Amber", "#F0A30A");
    m_universalMap.insert("Yellow", "#E3C800");
    m_universalMap.insert("Brown", "#825A2C");
    m_universalMap.insert("Olive", "#6D8764");
    m_universalMap.insert("Steel", "#647687");
    m_universalMap.insert("Mauve", "#76608A");
    m_universalMap.insert("Taupe", "#87794E");
}

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

bool AccentsListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Accent a = m_data[index.row()];
    switch (role) {
    case NAME:
        a.name = value.toString();
        break;
    case ACCENT:
        a.accent = value.toString();
        break;
    case LIGHT:
        a.light = value.toString();
        break;
    case QML:
        a.qml = value.toString();
        break;
    default:
        break;
    }

    m_data.replace(index.row(), a);

    emit dataChanged(index, index, {role});
    return true;
}

Qt::ItemFlags AccentsListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void AccentsListModel::add()
{
    Accent a;
    a.name = "New Accent";
    a.accent = "#000000";
    a.light = "#555555";
    a.qml = "Cobalt";

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data << a;
    endInsertRows();
}

bool AccentsListModel::remove(int row, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    m_data.remove(row);
    endRemoveRows();

    return true;
}

void AccentsListModel::save()
{
    static QString name = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QDir dir(name);
    dir.mkpath(BuildConfig.ORG_NAME);
    dir.cd(BuildConfig.ORG_NAME);

    QFile file(dir.absoluteFilePath("accents.json"));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Failed to open file" << name << "for writing.";
        return;
    }

    QTextStream stream(&file);
    stream << saveObject().toJson();
    file.close();
}

QJsonDocument AccentsListModel::saveObject() const
{
    QJsonArray arr;

    for (const Accent &a : m_data) {
        QJsonObject obj;

        obj.insert("name", a.name);
        obj.insert("accent", a.accent);
        obj.insert("light", a.light);
        obj.insert("qml", a.qml);

        arr.append(obj);
    }

    return QJsonDocument(arr);
}

void AccentsListModel::loadObject(const QJsonDocument &doc)
{
    QJsonArray arr = doc.array();

    for (const QJsonValueRef ref : arr) {
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
    dir.mkpath(BuildConfig.ORG_NAME);
    dir.cd(BuildConfig.ORG_NAME);

    QFile file(dir.absoluteFilePath("accents.json"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        file.setFileName(":/accents.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
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
    for (const Accent &a : m_data) {
        if (name == a.name) {
            return a.accent;
        }
    }

    return "";
}

QString AccentsListModel::light(const QString &name)
{
    for (const Accent &a : m_data) {
        if (name == a.name) {
            return a.light;
        }
    }

    return "";
}

QString AccentsListModel::qml(const QString &name)
{
    for (const Accent &a : m_data) {
        if (name == a.name) {
            return a.qml;
        }
    }

    return "";
}

QString AccentsListModel::qmlColor(const QString &name)
{
    return m_universalMap.value(name, "#3E65FF");
}

void AccentsListModel::copy(const QString &toCopy)
{
    QClipboard *clipboard = qApp->clipboard();
    clipboard->setText(toCopy);
}

void AccentsListModel::exportJson(const QString filename)
{
    QString name = filename;
    name.replace("file://", "");

    QFile file(name);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Failed to open file" << name << "for writing.";
        return;
    }

    QTextStream stream(&file);
    stream << saveObject().toJson();
    file.close();
}

void AccentsListModel::importJson(const QString filename)
{
    QString name = filename;
    name.replace("file://", "");

    QFile file(name);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Failed to open file" << name<< "for reading.";
        return;
    }

    QTextStream stream(&file);
    QByteArray data = stream.readAll().toUtf8();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    beginResetModel();
    m_data.clear();
    endResetModel();

    loadObject(doc);
    file.close();

    save();
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
    QHash<int,QByteArray> rez;
    rez[NAME] = "name";
    rez[ACCENT] = "accent";
    rez[LIGHT] = "light";
    rez[QML] = "qml";
    rez[IDX] = "idx";

    return rez;
}
