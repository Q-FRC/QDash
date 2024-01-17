#include "dialogs/CameraSelectionDialog.h"

#include <QKeyEvent>

CameraSelectionDialog::CameraSelectionDialog(QWidget *parent) : QDialog(parent)
{
    m_list = new QListWidget(this);

    setWindowTitle("Select Camera");

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::StandardButton::Ok | QDialogButtonBox::StandardButton::Cancel);
    m_layout = new QVBoxLayout(this);

    m_layout->addWidget(m_list, 1);
    m_layout->addWidget(m_buttonBox, 0);

    CameraStore::filterCameras();

    for (const Camera &camera : CameraStore::Cameras) {
        m_list->addItem(camera.Name);
    }

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, [this] {
        auto items = m_list->selectedItems();

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
    });

    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
