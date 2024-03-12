#ifndef CAMERAVIEWWIDGET_H
#define CAMERAVIEWWIDGET_H

#include <QVideoWidget>
#include <QMediaPlayer>
#include "widgets/BaseWidget.h"

class CameraViewWidget : public BaseWidget
{
    Q_OBJECT

    Q_PROPERTY(QUrl value READ url WRITE setUrl MEMBER m_url)
    Q_PROPERTY(QUrl URL READ url WRITE setUrl MEMBER m_url REQUIRED)
protected:
    QMediaPlayer *m_player;
    QVideoWidget *m_videoWidget;

    QUrl m_url = QUrl();
public:
    // blank param added to ensure meta type safety
    CameraViewWidget(const QString &title = "", const QUrl &url = QUrl());
    ~CameraViewWidget();

    QUrl url();
    void setUrl(const QUrl &url);

    void forceUpdate() override;
    void setConnected(bool connected = true) override;

    inline static WidgetTypes WidgetType = WidgetTypes::CameraView;
    inline static TopicTypes TopicType = TopicTypes::None;
    inline static QString SendableName = "";
    inline static QString DisplayName = "Camera View";
};

#endif // CAMERAVIEWWIDGET_H
