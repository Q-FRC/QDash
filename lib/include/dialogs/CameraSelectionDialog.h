#ifndef CAMERASELECTIONDIALOG_H
#define CAMERASELECTIONDIALOG_H

#include "stores/CameraStore.h"
#include <QDialog>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include "ui_CameraSelectionDialog.h"

class CameraSelectionDialog : public QDialog, Ui::CameraSelectionDialog
{
    Q_OBJECT
public:
    CameraSelectionDialog(QWidget *parent = nullptr);

public slots:
    void emitCamera();
signals:
    void selectedCamera(Camera camera);
};

#endif // CAMERASELECTIONDIALOG_H
