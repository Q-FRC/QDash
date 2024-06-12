// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <QGridLayout>
#include <QByteArray>
#include <QWidget>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QLabel>


class EventData : public QWidget
{
private:
    QLabel *redLabel(int row, int column);
    QLabel *blueLabel(int row, int column);

    QNetworkAccessManager *m_manager;

    QGridLayout *m_layout;

    QLabel *m_teamNumberLabel;

    QVBoxLayout *m_rankLayout;
    QLabel *m_rankNumberLabel;
    QLabel *m_rankScoreLabel;
    QLabel *m_recordLabel;

    QGridLayout *m_lastMatchLayout;
    QLabel *m_lastMatchCode;

    QList<QLabel *> m_redTeams;
    QLabel *m_red1Team;
    QLabel *m_red2Team;
    QLabel *m_red3Team;
    QLabel *m_redScore;

    QList<QLabel *> m_blueTeams;
    QLabel *m_blue1Team;
    QLabel *m_blue2Team;
    QLabel *m_blue3Team;
    QLabel *m_blueScore;
public:
    EventData();

    void updateRankingData();
    void updateEventData();
};
