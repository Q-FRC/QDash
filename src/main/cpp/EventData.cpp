// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "EventData.h"

#include <QNetworkReply>

EventData::EventData() {
    m_manager = new QNetworkAccessManager(this);
}

void EventData::test() {
    QNetworkRequest request;
    request.setUrl(QUrl("https://example.com"));
    request.setRawHeader("X-TBA-Auth-Key", AUTH_KEY);

    QNetworkReply *reply = m_manager->get(request);

    connect(reply, &QNetworkReply::readyRead, this, [=] {
        qDebug() << reply->read(reply->bytesAvailable());
    });
}
