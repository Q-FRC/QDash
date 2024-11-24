#ifndef MAPMODELMODEL_H
#define MAPMODELMODEL_H

#include <QAbstractTableModel>
#include <qqmlintegration.h>

typedef struct {
    QString key;
    QString value;
} Data;

class MapModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum MMRoleNames {
        KEY = Qt::UserRole + 1,
        VALUE
    };
    explicit MapModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_INVOKABLE void add(QString key = "", QString value = "");
    Q_INVOKABLE void remove(int row);
    Q_INVOKABLE QVariantList asList();
    Q_INVOKABLE void toList(const QVariantList &list);

    QString valueName() const;
    void setValueName(const QString &newValueName);

signals:
    void valueNameChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;


private:
    QList<Data> m_data;

    QString m_valueName;
    Q_PROPERTY(QString valueName READ valueName WRITE setValueName NOTIFY valueNameChanged FINAL)
};

#endif // MAPMODELMODEL_H
