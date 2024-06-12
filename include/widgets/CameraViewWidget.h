#ifndef CAMERAVIEWWIDGET_H
#define CAMERAVIEWWIDGET_H

#include "widgets/BaseWidget.h"
#include <QVideoWidget>
#include <QMediaPlayer>

class CameraViewWidget : public BaseWidget
{
protected:
    QMediaPlayer *m_player;
    QVideoWidget *m_videoWidget;

    QUrl m_url;
public:
    CameraViewWidget(const QString &title, const QUrl &url);
    virtual ~CameraViewWidget();

    QUrl url();
    void setUrl(const QUrl &url);
};

#endif // CAMERAVIEWWIDGET_H
