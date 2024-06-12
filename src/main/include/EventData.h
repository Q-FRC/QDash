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

inline void swap(QJsonValueRef v1, QJsonValueRef v2)
{
    QJsonValue temp(v1);
    v1 = QJsonValue(v2);
    v2 = temp;
}

class EventData : public QWidget
{
private:
    QLabel *redLabel(int row, int column);
    QLabel *blueLabel(int row, int column);

    const QJsonArray sortedMatches(QJsonArray matches);
    void setTeamNumbers(const QJsonValue match, QList<QLabel *> redTeamLabels, QList<QLabel *> blueTeamLabels);
    void setMatchScore(const QJsonValue match, QLabel *redScoreLabel, QLabel *blueScoreLabel);

    QNetworkAccessManager *m_manager;

    QGridLayout *m_layout;

    QLabel *m_teamNumberLabel;

    QVBoxLayout *m_rankLayout;
    QLabel *m_rankNumberLabel;
    QLabel *m_rankScoreLabel;
    QLabel *m_recordLabel;

    QGridLayout *m_lastMatchLayout;
    QLabel *m_lastMatchCode;

    QList<QLabel *> m_lastRedTeams;
    QLabel *m_lastRed1Team;
    QLabel *m_lastRed2Team;
    QLabel *m_lastRed3Team;
    QLabel *m_lastRedScore;

    QList<QLabel *> m_lastBlueTeams;
    QLabel *m_lastBlue1Team;
    QLabel *m_lastBlue2Team;
    QLabel *m_lastBlue3Team;
    QLabel *m_lastBlueScore;
public:
    EventData();

    void updateRankingData();
    void updateEventData();
};
