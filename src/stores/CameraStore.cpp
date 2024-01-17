#include "stores/CameraStore.h"
#include "Globals.h"

#include <QRegularExpression>

QList<Camera> CameraStore::Cameras{};

CameraStore::CameraStore()
{
    throw std::runtime_error("CameraStore is a static class.");
}

void CameraStore::filterCameras() {
    Cameras.clear();
    std::shared_ptr<nt::NetworkTable> table = Globals::inst.GetTable("/CameraPublisher");

    for (const std::string &st : table->GetSubTables()) {
        std::shared_ptr<nt::NetworkTable> subtable = table->GetSubTable(st);

        Camera camera = Camera::fromTable(subtable);
        Cameras.append(camera);
    }
}

Camera Camera::fromTable(std::shared_ptr<nt::NetworkTable> table) {
    Camera camera{};

    camera.Name = QString::fromStdString(table->GetEntry("description").GetString(""));
    camera.Source = QString::fromStdString(table->GetEntry("source").GetString(""));

    std::vector<std::string> streams = table->GetEntry("streams").GetStringArray({});

    for (const std::string &stream : streams) {
        QString qstream = QString::fromStdString(stream);
        qstream.replace(QRegularExpression("^(mjpe?g|ip|usb):"), "");
        qstream.replace("/?action=stream", "/stream.mjpg?");

        camera.Urls.append(QUrl(qstream));
    }

    return camera;
}

Camera CameraStore::getCameraFromName(QString name) {
    for (const Camera &camera : Cameras) {
        if (camera.Name == name) {
            return camera;
        }
    }

    throw std::runtime_error("Camera does not exist.");
}
