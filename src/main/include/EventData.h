// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <QByteArray>
#include <QWidget>
#include <QNetworkAccessManager>

const QByteArray AUTH_KEY = "";

class EventData : public QWidget
{
private:
    QNetworkAccessManager *m_manager;
public:
    EventData();

    void test();
};
