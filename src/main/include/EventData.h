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
    QLabel *redLabel(QGridLayout *layout, int row, int column);
    QLabel *blueLabel(QGridLayout *layout, int row, int column);

    const QJsonArray sortMatches(QJsonArray matches);
    int lastPlayedMatchIndex(const QJsonArray &matches);
    void setTeamNumbers(const QJsonValue match, QList<QLabel *> redTeamLabels, QList<QLabel *> blueTeamLabels);
    void setMatchScore(const QJsonValue match, QLabel *redScoreLabel, QLabel *blueScoreLabel);

    QNetworkAccessManager *m_manager;

    QGridLayout *m_layout;

    QLabel *m_teamNumberLabel;

    // RANK
    QVBoxLayout *m_rankLayout;
    QLabel *m_rankNumberLabel;
    QLabel *m_rankScoreLabel;
    QLabel *m_recordLabel;

    // LAST MATCH
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

    // CURRENT MATCH
    QGridLayout *m_currentMatchLayout;
    QLabel *m_currentMatchCode;

    QList<QLabel *> m_currentRedTeams;
    QLabel *m_currentRed1Team;
    QLabel *m_currentRed2Team;
    QLabel *m_currentRed3Team;

    QList<QLabel *> m_currentBlueTeams;
    QLabel *m_currentBlue1Team;
    QLabel *m_currentBlue2Team;
    QLabel *m_currentBlue3Team;

    // NEXT MATCH
    QGridLayout *m_nextMatchLayout;
    QLabel *m_nextMatchCode;

    QList<QLabel *> m_nextRedTeams;
    QLabel *m_nextRed1Team;
    QLabel *m_nextRed2Team;
    QLabel *m_nextRed3Team;

    QList<QLabel *> m_nextBlueTeams;
    QLabel *m_nextBlue1Team;
    QLabel *m_nextBlue2Team;
    QLabel *m_nextBlue3Team;
public:
    EventData();

    void updateRankingData();
    void updateLastMatchData();
    void updateCurrentMatchData();
    void updateNextMatchData();
};
