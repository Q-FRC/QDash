#ifndef GENERICMAPMODEL_H
#define GENERICMAPMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>

typedef struct
{
    QString key;
    QString value;
} Data;

class GenericMapModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum MMRoleTypes { KEY = Qt::UserRole, VALUE };

    GenericMapModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void add(QString key, QString value);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Data> m_data;
};

#endif // GENERICMAPMODEL_H
