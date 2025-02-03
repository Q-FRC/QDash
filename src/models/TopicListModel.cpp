#include "TopicListModel.h"


QList<QStandardItem*> recursiveSearch(QStandardItem *item, const QString &topic)
{
    QList<QStandardItem*> foundItems;

    // Check if the current item matches the search term
    if (item->data(TopicListModel::TOPIC) == topic) {
        foundItems.append(item);
    }

    // Recursively search through all child items
    for (int i = 0; i < item->rowCount(); ++i) {
        QStandardItem *childItem = item->child(i, 0);
        if (childItem) {
            foundItems.append(recursiveSearch(childItem, topic));
        }
    }

    return foundItems;
}

TopicListModel::TopicListModel(TopicStore &store, QObject *parent)
    : QStandardItemModel(parent)
    , m_store(&store)
{
    QHash<int, QByteArray> rez = QStandardItemModel::roleNames();
    rez.insert(TLMRoleTypes::NAME, "name");
    rez.insert(TLMRoleTypes::TYPE, "type");
    rez.insert(TLMRoleTypes::TOPIC, "topic");

    QStandardItemModel::setItemRoleNames(rez);
}

QVariant TopicListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == TLMRoleTypes::NAME) {
        return itemFromIndex(index)->text();
    }

    return QStandardItemModel::data(index, role);
}

void TopicListModel::reload()
{
    fetchMore(QModelIndex());
}

void TopicListModel::add(const QString &toAdd)
{
    if (toAdd.isEmpty() || toAdd == "/") return;

    QStringList split = toAdd.split('/');
    if (split.at(0).isEmpty()) split.remove(0);

    QStringList newList = split;
    newList.removeLast();
    QString parentPath('/' + newList.join('/'));

    nt::NetworkTableEntry type = Globals::inst.GetEntry(parentPath.toStdString() + "/.type");

    bool hasType = type.Exists();

    QStandardItem *parentItem = invisibleRootItem();

    QString total = "";
    for (const QString &sub : split) {
        total += "/" + sub;
        bool isLast = sub == split.last();

        auto results = recursiveSearch(invisibleRootItem(), total);

        if (results.isEmpty()) {
            QStandardItem *item = new QStandardItem(sub);
            item->setData(total, TOPIC);

            if (isLast) {
                if (hasType) {
                    if (sub == ".type") {
                        std::string value = type.GetString("invalid");

                        if (value == "invalid") {
                            QMetaObject::Connection *conn = new QMetaObject::Connection;

                            m_store->subscribe(toAdd);

                            *conn = connect(m_store, &TopicStore::topicUpdate, this, [conn, toAdd, parentItem, parentPath, this](QString topic, QVariant value) mutable {
                                if (topic == toAdd) {
                                    parentItem->setData(parentPath, TOPIC);
                                    QString typeStr = value.toString();
                                    parentItem->setData(typeStr, TYPE);

                                    m_store->unsubscribe(toAdd);

                                    disconnect(*conn);
                                    delete conn;
                                }
                            });
                        } else {
                            parentItem->setData(parentPath, TOPIC);
                            QString typeStr = QString::fromStdString(value);
                            parentItem->setData(typeStr, TYPE);
                        }

                        item->setData(toAdd, TLMRoleTypes::TOPIC);
                        item->setData(m_store->typeString(toAdd), TYPE);
                    }
                }
                item->setData(toAdd, TLMRoleTypes::TOPIC);
                item->setData(m_store->typeString(toAdd), TYPE);
            } else {
                if (parentItem && parentItem->text() == "CameraPublisher") {
                    item->setData("/CameraPublisher/" + sub, TLMRoleTypes::TOPIC);
                    item->setData("camera", TYPE);
                } else {
                    // item->setData("", TLMRoleTypes::TOPIC);
                    item->setData("", TYPE);
                }
            }

            parentItem->appendRow(item);
            parentItem = item;
        } else {
            for (QStandardItem *item : results) {

                if (item->parent() != nullptr && item->parent()->data(TLMRoleTypes::TYPE).toString() != "") goto end;

                if (item->parent() == nullptr || item->parent()->text() == parentItem->text()) {
                    parentItem = item;
                }
            }
        }
    }

end:
    type.Unpublish();
    return;
}

void TopicListModel::remove(const QString &toRemove)
{
    if (toRemove.isEmpty()) return;

    QStringList split = toRemove.split('/');
    if (split.at(0).isEmpty()) split.remove(0);

    QStandardItem *parentItem = invisibleRootItem();
    for (const QString &sub : split) {
        auto results = findItems(sub, Qt::MatchRecursive | Qt::MatchExactly | Qt::MatchWrap);

        if (results.isEmpty()) return;
        else {
            for (QStandardItem *item : results) {
                if (item == nullptr) continue;

                if (item->parent() == nullptr || item->parent() == parentItem) {
                    if (!item->hasChildren()) {
                        parentItem->removeRow(item->row());
                        if (!parentItem->hasChildren()) {
                            if (parentItem->parent() == nullptr) {
                                removeRow(parentItem->row());
                            } else {
                                parentItem->parent()->removeRow(parentItem->row());
                            }
                        }
                    }

                    parentItem = item;
                }
            }
        }
    }
}

