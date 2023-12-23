#include "misc/NewWidgetTreeDialog.h"

#include "stores/TypeStore.h"
#include "stores/FilterStore.h"

#include <QMenu>
#include <QKeyEvent>
#include <QHeaderView>

#include "Globals.h"

NewWidgetTreeDialog::NewWidgetTreeDialog(bool emitTopic, QWidget *parent) : QDialog(parent)
{
    m_emitTopic = emitTopic;

    FilterStore::filterTopics();
    m_layout = new QVBoxLayout(this);

    m_tree = new QTreeWidget(this);
    m_tree->setColumnCount(2);
    m_tree->header()->setStretchLastSection(false);
    m_tree->header()->setSectionResizeMode(0, QHeaderView::Stretch);

    QStringList headerLabels{};
    headerLabels << "Topic" << "Type";
    m_tree->setHeaderLabels(headerLabels);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::StandardButton::Cancel);

    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::close);

    m_layout->addWidget(m_tree);
    m_layout->addWidget(m_buttonBox);

    if (!emitTopic) {
        connect(&Globals::typeStore, &TypeStore::widgetReady, this, &NewWidgetTreeDialog::widgetReady);
        connect(&Globals::typeStore, &TypeStore::widgetReady, this, &NewWidgetTreeDialog::close);
    }
}

NewWidgetTreeDialog::~NewWidgetTreeDialog() {}

void NewWidgetTreeDialog::constructList(QList<Globals::Topic> topics) {
    m_tree->clear();

    for (const Globals::Topic & topic : topics)
    {
        createTreeIfNotExists(topic);

        if (m_emitTopic) {
            connect(m_tree, &QTreeWidget::itemActivated, this, [this, topic](QTreeWidgetItem *item) {
                if (getParentPath(item) == topic.name) {
                    emit topicReady(topic);
                    close();
                }
            });
        } else {
            QMenu *widgetMenu = Globals::typeStore.generateMenuForTopic(topic);

            connect(m_tree, &QTreeWidget::itemActivated, this, [this, topic, widgetMenu](QTreeWidgetItem *item) {
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
            m_tree->addTopLevelItem(item);

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
    item->setFirstColumnSpanned(true);

    if (superTable.isEmpty()) {
        m_tree->addTopLevelItem(item);
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

QList<Globals::Topic> NewWidgetTreeDialog::filterNumberTypes(QList<Globals::Topic> list) {
    QList<Globals::Topic> newList{};

    QList<TopicTypes> acceptableTypes = {
        TopicTypes::Int,
        TopicTypes::Double,
        TopicTypes::Boolean
    };

    for (const Globals::Topic &topic : list) {
        if (acceptableTypes.contains(topic.type)) {
            newList.append(topic);
        }
    }

    return newList;
}

void NewWidgetTreeDialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        return;
    }
    QDialog::keyPressEvent(event);
}
