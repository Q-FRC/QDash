#ifndef NEWWIDGETTREEDIALOG_H
#define NEWWIDGETTREEDIALOG_H

#include <QDialog>

#include <QVBoxLayout>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QTreeWidget>

#include "Globals.h"
#include "widgets/BaseWidget.h"
#include "ui_NewWidgetTreeDialog.h"

class NewWidgetTreeDialog : public QDialog, Ui::NewWidgetTreeDialog
{
    Q_OBJECT
private:
    QMap<QString, QTreeWidgetItem *> m_itemTableMap;

    bool m_emitTopic = false;

    void createTreeIfNotExists(const Globals::Topic &topic);

    QString getParentPath(QTreeWidgetItem *item);

public:
    explicit NewWidgetTreeDialog(bool emitTopic = false, QWidget *parent = nullptr);
    virtual ~NewWidgetTreeDialog();
    
    void keyPressEvent(QKeyEvent *event);
    void constructList(QList<Globals::Topic> topics);

    static QList<Globals::Topic> filterNumberTypes(QList<Globals::Topic> list);
signals:
    void widgetReady(BaseWidget *widget, WidgetData data);
    void topicReady(const Globals::Topic &topic);
};

#endif // NEWWIDGETTREEDIALOG_H
