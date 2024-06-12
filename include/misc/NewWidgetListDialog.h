#ifndef NEWWIDGETLISTDIALOG_H
#define NEWWIDGETLISTDIALOG_H

#include <QDialog>

#include <QVBoxLayout>
#include <QListWidget>
#include <QDialogButtonBox>

#include "Globals.h"
#include "widgets/BaseWidget.h"

class NewWidgetListDialog : public QDialog
{
    Q_OBJECT
private:
    QVBoxLayout *m_layout;
    QListWidget *m_list;

    QDialogButtonBox *m_buttonBox;

    void constructList();
    QAction *createWidgetAction(const QString &text, const QString &ntTopic, const WidgetTypes &widgetType);
public:
    NewWidgetListDialog(QWidget *parent);
    virtual ~NewWidgetListDialog();

    void showNewWidgetDialog(WidgetTypes widgetType, std::string ntTopic);

    void keyPressEvent(QKeyEvent *event);

public slots:
    void emitWidget(BaseWidget *widget, WidgetData data);

signals:
    void widgetReady(BaseWidget *widget, WidgetData data);
};

#endif // NEWWIDGETLISTDIALOG_H
