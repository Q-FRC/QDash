#include "misc/NewWidgetListDialog.h"
#include "dialogs/NewWidgetDialog.h"

#include <QMenu>
#include <QKeyEvent>

#include "Globals.h"

NewWidgetListDialog::NewWidgetListDialog() : QDialog()
{
    m_layout = new QVBoxLayout(this);

    m_list = new QListWidget(this);
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::StandardButton::Cancel);

    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::close);

    m_layout->addWidget(m_list);
    m_layout->addWidget(m_buttonBox);

    constructList();
}

NewWidgetListDialog::~NewWidgetListDialog() {}

void NewWidgetListDialog::constructList() {
    m_list->clear();
    QMapIterator<QString, Globals::TopicTypes> iterator(Globals::availableTopics);

    while (iterator.hasNext())
    {
        iterator.next();
        QString topicName = iterator.key();
        Globals::TopicTypes topicType = iterator.value();

        m_list->addItem(topicName);

        switch(topicType) {
        case Globals::TopicTypes::Boolean: {
            QMenu *boolMenu = new QMenu(topicName);

            QAction *checkboxAction = createWidgetAction("Checkbox", topicName, NewWidgetDialog::WidgetTypes::BooleanCheckbox);
            boolMenu->addAction(checkboxAction);

            QAction *colorAction = createWidgetAction("Color Display", topicName, NewWidgetDialog::WidgetTypes::BooleanDisplay);
            boolMenu->addAction(colorAction);

            connect(m_list, &QListWidget::itemActivated, this, [this, topicName, boolMenu](QListWidgetItem *item) {
                if (item->text() == topicName) {
                    boolMenu->popup(QCursor::pos());
                }
            });
            break;
        }
        case Globals::TopicTypes::Double: {
            QMenu *doubleMenu = new QMenu(topicName);

            QAction *displayAction = createWidgetAction("Number Display", topicName, NewWidgetDialog::WidgetTypes::DoubleDisplay);
            doubleMenu->addAction(displayAction);

            QAction *dialAction = createWidgetAction("Dial", topicName, NewWidgetDialog::WidgetTypes::DoubleDial);
            doubleMenu->addAction(dialAction);

            connect(m_list, &QListWidget::itemActivated, this, [this, topicName, doubleMenu](QListWidgetItem *item) {
                if (item->text() == topicName) {
                    doubleMenu->popup(QCursor::pos());
                }
            });
            break;
        }
        case Globals::TopicTypes::SendableChooser: {
            connect(m_list, &QListWidget::itemActivated, this, [this, topicName](QListWidgetItem *item) {
                if (item->text() == topicName) {
                    showNewWidgetDialog(NewWidgetDialog::WidgetTypes::SendableChooser, topicName.toStdString());
                }
            });
            break;
        }
        case Globals::TopicTypes::String:
        default: {
            connect(m_list, &QListWidget::itemActivated, this, [this, topicName](QListWidgetItem *item) {
                if (item->text() == topicName) {
                    showNewWidgetDialog(NewWidgetDialog::WidgetTypes::StringDisplay, topicName.toStdString());
                }
            });
            break;
        }
        }
    }
}

void NewWidgetListDialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
//        QList<QListWidgetItem *> items = m_list->selectedItems();
//        if (!items.empty()) {
//            emit m_list->itemActivated(items.at(0));
//        }
        return;
    }
    QDialog::keyPressEvent(event);
}

QAction *NewWidgetListDialog::createWidgetAction(const QString &text, const QString &ntTopic, const NewWidgetDialog::WidgetTypes &widgetType) {
    QAction *action = new QAction(text, this);

    connect(action, &QAction::triggered, this, [this, text, ntTopic, widgetType](bool) {
        showNewWidgetDialog(widgetType, ntTopic.toStdString());
    });

    return action;
}

void NewWidgetListDialog::showNewWidgetDialog(NewWidgetDialog::WidgetTypes widgetType, std::string ntTopic) {
    NewWidgetDialog *dialog = NewWidgetDialog::fromWidgetType(widgetType, ntTopic);
    dialog->show();

    connect(dialog, &NewWidgetDialog::widgetReady, this, &NewWidgetListDialog::emitWidget);
    close();
}

void NewWidgetListDialog::emitWidget(BaseWidget *widget, WidgetData data) {
    emit widgetReady(widget, data);
}
