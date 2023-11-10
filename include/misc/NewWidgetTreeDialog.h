#ifndef NEWWIDGETTREEDIALOG_H
#define NEWWIDGETTREEDIALOG_H

#include <QDialog>

#include <QVBoxLayout>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QTreeWidget>

#include "Globals.h"
#include "widgets/BaseWidget.h"

class NewWidgetTreeDialog : public QDialog
{
    Q_OBJECT
private:
    QVBoxLayout *m_layout;
    QTreeWidget *m_tree;

    QDialogButtonBox *m_buttonBox;

    QMap<QString, QTreeWidgetItem *> m_itemTableMap;

    void constructList();
    QAction *createWidgetAction(const QString &text, const QString &ntTopic, const WidgetTypes &widgetType);
    void createTreeIfNotExists(QString topicName, Globals::TopicTypes type);

    QString getParentPath(QTreeWidgetItem *item);
public:
    NewWidgetTreeDialog(QWidget *parent);
    virtual ~NewWidgetTreeDialog();

    void showNewWidgetDialog(WidgetTypes widgetType, std::string ntTopic);

    void keyPressEvent(QKeyEvent *event);

public slots:
    void emitWidget(BaseWidget *widget, WidgetData data);

signals:
    void widgetReady(BaseWidget *widget, WidgetData data);
};

#endif // NEWWIDGETTREEDIALOG_H
