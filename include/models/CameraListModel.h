#ifndef CAMERALISTMODEL_H
#define CAMERALISTMODEL_H

#include "TopicStore.h"
#include "networktables/NetworkTable.h"
#include <QAbstractListModel>
#include <qqmlintegration.h>
#include <qurl.h>

typedef struct Camera
{
public:
    static struct Camera fromTable(std::shared_ptr<nt::NetworkTable> table);

    QList<QUrl> urls;
    QString name;
    QString source;
} Camera;

class CameraListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum CLMRoleTypes {
        NAME = Qt::UserRole,
        URLS,
        SOURCE
    };

    explicit CameraListModel(TopicStore &store, QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    Q_INVOKABLE void add(std::shared_ptr<nt::NetworkTable> table);

    // Remove data:
    void clear();
    Q_INVOKABLE bool remove(int row, const QModelIndex &parent = QModelIndex());

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Camera> m_data;

    TopicStore *m_store;
};

#endif // CAMERALISTMODEL_H
