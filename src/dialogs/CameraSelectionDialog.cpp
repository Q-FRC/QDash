#include "dialogs/CameraSelectionDialog.h"

#include <QKeyEvent>

CameraSelectionDialog::CameraSelectionDialog(QWidget *parent) : QDialog(parent), Ui::CameraSelectionDialog()
{
    setupUi(this);
    CameraStore::filterCameras();

    for (const Camera &camera : CameraStore::Cameras) {
        listWidget->addItem(camera.Name);
    }
}

void CameraSelectionDialog::emitCamera() {
    auto items = listWidget->selectedItems();

    if (items.empty()) return;
    QString selected = items.at(0)->text();

    Camera camera;
    try {
        camera = CameraStore::getCameraFromName(selected);
    } catch (std::exception &e) {
        qCritical() << "uh oh stinky";
        return;
    }

    emit selectedCamera(camera);
    accept();
}
