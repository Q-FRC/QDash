#ifndef ACCENTSLISTMODEL_H
#define ACCENTSLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>
#include <QJsonDocument>

typedef struct Accent {
    QString name;
    QString accent;
    QString light;
    QString qml;
} Accent;

class AccentsListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum ALMRoleTypes {
        NAME = Qt::UserRole + 1,
        ACCENT,
        LIGHT,
        QML,
        IDX
    };

    explicit AccentsListModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    Q_INVOKABLE void add();

    // Remove data:
    Q_INVOKABLE bool remove(int row, const QModelIndex &parent = QModelIndex());

    Q_INVOKABLE void save();
    Q_INVOKABLE QJsonDocument saveObject() const;
    Q_INVOKABLE void loadObject(const QJsonDocument &doc);
    Q_INVOKABLE void load();

    Q_INVOKABLE QString accent(const QString &name);
    Q_INVOKABLE QString light(const QString &name);
    Q_INVOKABLE QString qml(const QString &name);

    Q_INVOKABLE void copy(const QString &toCopy);

    Q_INVOKABLE void exportJson(const QString filename);
    Q_INVOKABLE void importJson(const QString filename);

    Q_INVOKABLE QStringList names() const;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Accent> m_data;
};

#endif // ACCENTSLISTMODEL_H
