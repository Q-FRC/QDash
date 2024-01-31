#ifndef CAMERASTORE_H
#define CAMERASTORE_H

#include <QObject>
#include <QUrl>
#include "networktables/NetworkTable.h"

class Camera
{
private:
    QUrl m_urls{};
    QString m_name{};
    QString m_source{};
public:
    Camera() = default;

    static Camera fromTable(std::shared_ptr<nt::NetworkTable> table);

    QList<QUrl> Urls;
    QString Name;
    QString Source;
};

class CameraStore : public QObject
{
    Q_OBJECT
private:
public:
    CameraStore();

    static QList<Camera> Cameras;
    static void filterCameras();
    static Camera getCameraFromName(QString name);

signals:
};

#endif // CAMERASTORE_H
