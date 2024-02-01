#include "dialogs/NewWidgetTreeDialog.h"

#include "stores/TypeStore.h"
#include "stores/FilterStore.h"

#include <QMenu>
#include <QKeyEvent>
#include <QHeaderView>

#include "Globals.h"

NewWidgetTreeDialog::NewWidgetTreeDialog(bool emitTopic, QWidget *parent) : QDialog(parent), Ui::NewWidgetTreeDialog()
{
    setupUi(this);
    m_emitTopic = emitTopic;

    tree->header()->setSectionResizeMode(0, QHeaderView::Stretch);

    FilterStore::filterTopics();

    if (!emitTopic) {
        connect(&Globals::typeStore, &TypeStore::widgetReady, this, &NewWidgetTreeDialog::widgetReady);
        connect(&Globals::typeStore, &TypeStore::widgetReady, this, &NewWidgetTreeDialog::close);
    }
}

NewWidgetTreeDialog::~NewWidgetTreeDialog() {}

void NewWidgetTreeDialog::constructList(QList<Globals::Topic> topics) {
    tree->clear();

    for (const Globals::Topic & topic : topics)
    {
        createTreeIfNotExists(topic);

        if (m_emitTopic) {
            connect(tree, &QTreeWidget::itemActivated, this, [this, topic](QTreeWidgetItem *item) {
                if (getParentPath(item) == topic.name) {
                    emit topicReady(topic);
                    close();
                }
            });
        } else {
            QMenu *widgetMenu = Globals::typeStore.generateMenuForTopic(topic);

            connect(tree, &QTreeWidget::itemActivated, this, [this, topic, widgetMenu](QTreeWidgetItem *item) {
                if (getParentPath(item) == topic.name) {
                    widgetMenu->popup(QCursor::pos());
                }
            });
        }
    }
}

void NewWidgetTreeDialog::createTreeIfNotExists(const Globals::Topic &topic) {
    QStringList split = topic.name.split('/');
    if (!split.at(0).isEmpty()) { // protect against entries not prefixed with /
        split.prepend("");
    }

    QStringList tablePath = split.sliced(0, split.length() - 1);

    // The first entry will always be a blank string
    for (int i = 1; i < tablePath.length(); ++i) {
        QString table = tablePath.sliced(0, i + 1).join("/");
        QString superTable = tablePath.sliced(0, i).join("/");

        if (m_itemTableMap.contains(table)) continue;

        QStringList columns{};
        columns << tablePath.at(i) << "";

        QTreeWidgetItem *item = new QTreeWidgetItem(columns);
        item->setFirstColumnSpanned(true);

        if (i == 1) {
            m_itemTableMap.insert("/" + tablePath.at(1), item);
            tree->addTopLevelItem(item);

            continue;
        }

        QTreeWidgetItem *parent = m_itemTableMap.value(superTable);

        if (parent == nullptr) {
            qCritical() << "something bad happened with" << table << superTable;
            continue;
        }

        parent->addChild(item);
        m_itemTableMap.insert(table, item);
    }

    QString topicEnd = split.last();
    QString superTable = tablePath.join('/');

    QStringList columns{};
    columns << topicEnd << Globals::topicTypeDisplayNames.value(topic.type);

    QTreeWidgetItem *item = new QTreeWidgetItem(columns);
    item->setToolTip(1, columns.at(1));
    item->setFirstColumnSpanned(true);

    if (superTable.isEmpty()) {
        tree->addTopLevelItem(item);
        return;
    }

    QTreeWidgetItem *parent = m_itemTableMap.value(superTable);

    if (parent == nullptr) {
        qCritical() << "something very bad happened with topic" << topicEnd << superTable;
        return;
    }

    parent->addChild(item);
}

QString NewWidgetTreeDialog::getParentPath(QTreeWidgetItem *item) {
    QTreeWidgetItem *parent = item->parent();
    QStringList list{};
    list << item->text(0);

    while (parent != nullptr) {
        list.prepend(parent->text(0));
        parent = parent->parent();
    }

    return "/" + list.join('/');
}

QList<Globals::Topic> NewWidgetTreeDialog::filterTopicTypes(QList<Globals::Topic> list, QList<TopicTypes> acceptableTypes) {
    QList<Globals::Topic> newList{};

    for (const Globals::Topic &topic : list) {
        if (acceptableTypes.contains(topic.type)) {
            newList.append(topic);
        }
    }

    return newList;
}

QList<Globals::Topic> NewWidgetTreeDialog::filterNumberTypes(QList<Globals::Topic> list) {
    QList<TopicTypes> acceptableTypes = {
        TopicTypes::Int,
        TopicTypes::Double,
        TopicTypes::Boolean
    };

    return filterTopicTypes(list, acceptableTypes);
}

QList<Globals::Topic> NewWidgetTreeDialog::filterStringTypes(QList<Globals::Topic> list) {
    QList<TopicTypes> acceptableTypes = {
        TopicTypes::String
    };

    return filterTopicTypes(list, acceptableTypes);
}

void NewWidgetTreeDialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        return;
    }
    QDialog::keyPressEvent(event);
}
