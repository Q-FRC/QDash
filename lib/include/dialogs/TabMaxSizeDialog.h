#ifndef TABMAXSIZEDIALOG_H
#define TABMAXSIZEDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QFormLayout>
#include <QDialogButtonBox>

#include "ui_TabMaxSizeDialog.h"

class TabMaxSizeDialog : public QDialog, Ui::TabMaxSizeDialog
{
    Q_OBJECT
public:
    TabMaxSizeDialog(QWidget *parent = nullptr, const QPoint &maxSize = QPoint());
    ~TabMaxSizeDialog();

public slots:
    void emitData();
signals:
    void dataReady(QPoint point);
};

#endif // TABMAXSIZEDIALOG_H
