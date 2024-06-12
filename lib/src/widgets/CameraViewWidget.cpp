 #include "widgets/CameraViewWidget.h"

#include <QMenu>
#include <QThread>

CameraViewWidget::CameraViewWidget(const QString &title, const QUrl &url) : BaseWidget(WidgetTypes::CameraView, title, "", true)
{
    m_videoWidget = new QVideoWidget(this);

    m_player = new QMediaPlayer(this);
    m_player->setVideoOutput(m_videoWidget);

    setUrl(url);

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

    m_player->stop();

    QThread::msleep(100);

    m_player->setSource(url);
    m_player->play();
}

void CameraViewWidget::forceUpdate() {
    m_player->play();
}

void CameraViewWidget::setConnected(bool connected) {
    if (connected) setUrl(m_url);
    else m_player->stop();
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
