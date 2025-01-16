#ifndef TOPICLISTMODEL_H
#define TOPICLISTMODEL_H

#include "TopicStore.h"
#include <QStandardItemModel>

class TopicListModel : public QStandardItemModel
{
    Q_OBJECT

public:
    enum TLMRoleTypes {
        NAME = Qt::UserRole,
        TYPE,
        TOPIC
    };

    TopicListModel(TopicStore &store, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE void reload();

    void add(const QString &toAdd);
    void remove(const QString &toRemove);

private:
    QStringList m_data;
    TopicStore *m_store;
};

#endif // MODELSTOPICLISTMODEL_H
