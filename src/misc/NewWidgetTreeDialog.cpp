#include "misc/NewWidgetTreeDialog.h"

#include "misc/WidgetDialogGenerator.h"

#include "stores/TypeStore.h"
#include "stores/FilterStore.h"

#include <QMenu>
#include <QKeyEvent>
#include <QHeaderView>

#include "Globals.h"

NewWidgetTreeDialog::NewWidgetTreeDialog(QWidget *parent) : QDialog(parent)
{
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

    constructList();

    connect(Globals::typeStore, &TypeStore::widgetReady, this, &NewWidgetTreeDialog::emitWidget);
    connect(Globals::typeStore, &TypeStore::dialogShown, this, &QDialog::close);
}

NewWidgetTreeDialog::~NewWidgetTreeDialog() {}

void NewWidgetTreeDialog::constructList() {
    m_tree->clear();
    QMapIterator<QString, TopicTypes> iterator(FilterStore::FilteredTopics);

    while (iterator.hasNext())
    {
        iterator.next();
        QString topicName = iterator.key();
        TopicTypes topicType = iterator.value();

        createTreeIfNotExists(topicName, topicType);

        QMenu *widgetMenu = Globals::typeStore->generateMenuForType(topicType, topicName.toStdString());

        connect(m_tree, &QTreeWidget::itemActivated, this, [this, topicName, widgetMenu](QTreeWidgetItem *item) {
            if (getParentPath(item) == topicName) {
                widgetMenu->popup(QCursor::pos());
            }
        });
    }
}

void NewWidgetTreeDialog::createTreeIfNotExists(QString topicName, TopicTypes type) {
    QStringList split = topicName.split('/');
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

    QString topic = split.last();
    QString superTable = tablePath.join('/');

    QStringList columns{};
    columns << topic << Globals::topicTypeDisplayNames.value(type);

    QTreeWidgetItem *item = new QTreeWidgetItem(columns);
    item->setFirstColumnSpanned(true);

    if (superTable.isEmpty()) {
        m_tree->addTopLevelItem(item);
        return;
    }

    QTreeWidgetItem *parent = m_itemTableMap.value(superTable);

    if (parent == nullptr) {
        qCritical() << "something very bad happened with topic" << topic << superTable;
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

void NewWidgetTreeDialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        return;
    }
    QDialog::keyPressEvent(event);
}

QAction *NewWidgetTreeDialog::createWidgetAction(const QString &text, const QString &ntTopic, const WidgetTypes &widgetType) {
    QAction *action = new QAction(text, this);

    connect(action, &QAction::triggered, this, [this, text, ntTopic, widgetType](bool) {
        showNewWidgetDialog(widgetType, ntTopic.toStdString());
    });

    return action;
}

void NewWidgetTreeDialog::showNewWidgetDialog(WidgetTypes widgetType, std::string ntTopic) {
    auto widget = BaseWidget::defaultWidgetFromTopic(QString::fromStdString(ntTopic), widgetType);

    WidgetDialogGenerator *dialog = new WidgetDialogGenerator(widget, this->parentWidget());
    dialog->setWindowTitle("New Widget");
    dialog->show();

    connect(dialog, &WidgetDialogGenerator::widgetReady, this, &NewWidgetTreeDialog::emitWidget);
    close();
}

void NewWidgetTreeDialog::emitWidget(BaseWidget *widget, WidgetData data) {
    emit widgetReady(widget, data);
}
