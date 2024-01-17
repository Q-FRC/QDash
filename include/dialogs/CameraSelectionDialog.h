#ifndef CAMERASELECTIONDIALOG_H
#define CAMERASELECTIONDIALOG_H

#include "stores/CameraStore.h"
#include <QDialog>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QVBoxLayout>

class CameraSelectionDialog : public QDialog
{
    Q_OBJECT
private:
    QListWidget *m_list;
    QDialogButtonBox *m_buttonBox;

    QVBoxLayout *m_layout;
public:
    CameraSelectionDialog(QWidget *parent = nullptr);

signals:
    void selectedCamera(Camera camera);
};

#endif // CAMERASELECTIONDIALOG_H
