 #include "widgets/CameraViewWidget.h"

#include <QMenu>

CameraViewWidget::CameraViewWidget(const QString &title, const QUrl &url) : BaseWidget(WidgetTypes::CameraView, title, "", true)
{
    m_videoWidget = new QVideoWidget(this);

    m_player = new QMediaPlayer(this);
    m_url = url;

    m_player->setSource(url);
    m_player->setVideoOutput(m_videoWidget);

    m_player->play();

    m_layout->addWidget(m_videoWidget, 1, 0, 4, 1);
    setReady(true);
}

CameraViewWidget::~CameraViewWidget() {
    m_player->stop();
}

QUrl CameraViewWidget::url() {
    return m_player->source();
}

void CameraViewWidget::setUrl(const QUrl &url) {
    m_url = url;

    m_player->setSource(url);
    m_player->play();
}

void CameraViewWidget::forceUpdate() {
    // setUrl(m_url);
    m_player->play();
}

void CameraViewWidget::setConnected(bool connected) {
    if (connected) setUrl(m_url);
}

QMenu *CameraViewWidget::constructContextMenu(WidgetData data) {
    QMenu *menu = BaseWidget::constructContextMenu(data);

    QAction *reconnect = new QAction("Reconnect", menu);

    connect(reconnect, &QAction::triggered, this, [this] {
        setUrl(m_url);
    });

    menu->addAction(reconnect);

    return menu;
}
