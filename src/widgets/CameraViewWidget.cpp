#include "widgets/CameraViewWidget.h"

CameraViewWidget::CameraViewWidget(const QString &title, const QUrl &url) : BaseWidget(title, "")
{
    m_videoWidget = new QVideoWidget(this);

    m_player = new QMediaPlayer;
    m_player->setSource(url);
    m_player->setVideoOutput(m_videoWidget);

    m_player->play();

    m_layout->addWidget(m_videoWidget, 1, 0, 1, 4);
}

CameraViewWidget::~CameraViewWidget() {}

QUrl CameraViewWidget::url() {
    return m_player->source();
}

void CameraViewWidget::setUrl(const QUrl &url) {
    m_player->setSource(url);
    m_player->play();
}

QJsonObject CameraViewWidget::saveObject() {
    QJsonObject object = BaseWidget::saveObject();

    object.insert("url", url().toString());
    object.insert("widgetType", (int) WidgetTypes::CameraView);

    return object;
}
