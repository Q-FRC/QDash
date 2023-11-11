#ifndef CAMERAVIEWWIDGET_H
#define CAMERAVIEWWIDGET_H

#include <QVideoWidget>
#include <QMediaPlayer>
#include "widgets/BaseWidget.h"

class CameraViewWidget : public BaseWidget
{
protected:
    QMediaPlayer *m_player;
    QVideoWidget *m_videoWidget;

    QUrl m_url;
public:
    CameraViewWidget(const QString &title, const QUrl &url);
    ~CameraViewWidget();

    QUrl url();
    void setUrl(const QUrl &url);

    QJsonObject saveObject() override;
};

#endif // CAMERAVIEWWIDGET_H
