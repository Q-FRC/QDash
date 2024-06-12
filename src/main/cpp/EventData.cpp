// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "EventData.h"
#include "Constants.h"

#include <QNetworkReply>
#include <QJsonArray>

#include <algorithm>

// Set the label's font size.
void setFontSize(QLabel *label, int pixelSize)
{
    QFont font = label->font();
    font.setPixelSize(pixelSize);
    label->setFont(font);
}

// Set the label's font size.
void setFontBold(QLabel *label)
{
    QFont font = label->font();
    font.setBold(true);
    label->setFont(font);
}

// Set the label's font size.
void setFontUnderline(QLabel *label)
{
    QFont font = label->font();
    font.setUnderline(true);
    label->setFont(font);
}

// Set the label's font color. (use hex color, i.e. #EEEEFF)
void setFontColor(QLabel *label, QString color)
{
    label->setStyleSheet("color: " + color + ";");
}

// Gets a QLabel for a red team/score and adds it to the match layout at the specified position.
QLabel *EventData::redLabel(int row, int column)
{
    QLabel *label = new QLabel("180", this);
    label->setStyleSheet("background-color: #FFEEEE;\n"
                         "border: 1px solid gray;\n"
                         "color: #222020;");
    setFontSize(label, 20);

    m_lastMatchLayout->addWidget(label, row, column);
    return label;
}

// Gets a QLabel for a blue team/score and adds it to the match layout at the specified position.
QLabel *EventData::blueLabel(int row, int column)
{
    QLabel *label = new QLabel("180", this);
    label->setStyleSheet("background-color: #EEEEFF;\n"
                         "border: 1px solid gray;\n"
                         "color: #222020;");
    setFontSize(label, 20);

    m_lastMatchLayout->addWidget(label, row, column);
    return label;
}

// Sort matches by time, earliest first.
const QJsonArray EventData::sortedMatches(QJsonArray matches)
{

    std::sort(matches.begin(), matches.end(), [](const QJsonValue &value1, const QJsonValue &value2)
              {
                  return value1["time"].toInteger() < value2["time"].toInteger();
              });

    return matches;
}

// Set the specified list of labels to use the team numbers for a match.
void EventData::setTeamNumbers(const QJsonValue match, QList<QLabel *> redTeamLabels, QList<QLabel *> blueTeamLabels)
{
    // Alliances
    const QJsonValue alliances = match["alliances"];
    const QJsonValue red = alliances["red"];
    const QJsonValue blue = alliances["blue"];

    const QString winningAlliance = match["winning_alliance"].toString();

    // Teams
    // Red
    const QJsonArray redTeams = red["team_keys"].toArray();

    for (qsizetype i = 0; i < redTeams.size(); ++i)
    {
        QString key = redTeams[i].toString();
        QLabel *label = redTeamLabels[i];

        if (label == nullptr)
        {
            continue;
        }

        label->setText(key.mid(3, 5));

        if (key == Constants::TEAM_NUMBER)
        {
            setFontUnderline(label);
        }

        if (winningAlliance == "red")
        {
            setFontBold(label);
        }
    }

    // blue
    const QJsonArray blueTeams = blue["team_keys"].toArray();

    for (qsizetype i = 0; i < blueTeams.size(); ++i)
    {
        QString key = blueTeams[i].toString();
        QLabel *label = blueTeamLabels[i];

        label->setText(key.mid(3, 5));

        if (key == Constants::TEAM_NUMBER)
        {
            setFontUnderline(label);
        }

        if (winningAlliance == "blue")
        {
            setFontBold(label);
        }
    }
}

// Set the specified labels to the match score.
void EventData::setMatchScore(const QJsonValue match, QLabel *redScoreLabel, QLabel *blueScoreLabel)
{
    // Alliances
    const QJsonValue alliances = match["alliances"];
    const QJsonValue red = alliances["red"];
    const QJsonValue blue = alliances["blue"];

    // Scores & Winner
    const QString redScore = QString::number(red["score"].toInteger());
    const QString blueScore = QString::number(blue["score"].toInteger());
    const QString winningAlliance = match["winning_alliance"].toString();

    if (winningAlliance == "red")
    {
        setFontBold(redScoreLabel);
    }
    else if (winningAlliance == "blue")
    {
        setFontBold(blueScoreLabel);
    }

    redScoreLabel->setText(redScore);
    blueScoreLabel->setText(blueScore);
}

EventData::EventData()
{
    m_manager = new QNetworkAccessManager(this);

    // MAIN LAYOUT & TEAM NUMBER
    m_layout = new QGridLayout(this);

    m_teamNumberLabel = new QLabel(Constants::TEAM_NUMBER, this);
    setFontSize(m_teamNumberLabel, 45);

    m_layout->addWidget(m_teamNumberLabel, 0, 0, 1, 2, Qt::AlignCenter | Qt::AlignBottom);

    // RANK LAYOUT
    m_rankLayout = new QVBoxLayout;

    m_rankNumberLabel = new QLabel("Rank 27", this);
    setFontSize(m_rankNumberLabel, 35);
    m_rankLayout->addWidget(m_rankNumberLabel);

    m_rankScoreLabel = new QLabel("2.54 RS", this);
    setFontSize(m_rankScoreLabel, 25);
    m_rankLayout->addWidget(m_rankScoreLabel);

    m_recordLabel = new QLabel("1-8-0 Qual Record", this);
    setFontSize(m_recordLabel, 24);
    m_rankLayout->addWidget(m_recordLabel);

    m_layout->addLayout(m_rankLayout, 1, 0, Qt::AlignCenter | Qt::AlignTop);

    // LAST MATCH LAYOUT
    m_lastMatchLayout = new QGridLayout;
    m_lastMatchLayout->setSpacing(0);

    // Code
    m_lastMatchCode = new QLabel("Last Match: qm48", this);
    setFontSize(m_lastMatchCode, 40);
    m_lastMatchLayout->addWidget(m_lastMatchCode, 0, 0, 1, 4, Qt::AlignCenter);

    // Red Teams
    m_lastRed1Team = redLabel(1, 0);
    m_lastRed2Team = redLabel(1, 1);
    m_lastRed3Team = redLabel(1, 2);
    m_lastRedScore = redLabel(1, 3);

    m_lastRedTeams = QList{m_lastRed1Team, m_lastRed2Team, m_lastRed3Team};

    // Blue Teams
    m_lastBlue1Team = blueLabel(2, 0);
    m_lastBlue2Team = blueLabel(2, 1);
    m_lastBlue3Team = blueLabel(2, 2);
    m_lastBlueScore = blueLabel(2, 3);

    m_lastBlueTeams = QList{m_lastBlue1Team, m_lastBlue2Team, m_lastBlue3Team};

    m_layout->addLayout(m_lastMatchLayout, 1, 1, Qt::AlignCenter | Qt::AlignTop);
}

void EventData::updateRankingData()
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://thebluealliance.com/api/v3/team/" + Constants::TEAM_NUMBER + "/event/" + Constants::EVENT_CODE + "/status"));
    request.setRawHeader("X-TBA-Auth-Key", Constants::AUTH_KEY);

    QNetworkReply *reply = m_manager->get(request);

    connect(reply, &QNetworkReply::readyRead, this, [this, reply]
            {
        const QByteArray data = reply->read(reply->bytesAvailable());

        const QJsonDocument json = QJsonDocument::fromJson(data);
        const QJsonValue ranking = json["qual"]["ranking"];

        // Get values
        const QString rank = QString::number(ranking["rank"].toDouble());
        const QString rankScore = QString::number(ranking["sort_orders"][0].toDouble());

        const QJsonValue recordJson = ranking["record"];
        const QString record = QString::number(recordJson["wins"].toDouble()) + "-" + QString::number(recordJson["losses"].toDouble()) + "-" + QString::number(recordJson["ties"].toDouble());

        m_rankNumberLabel->setText("Rank " + rank);
        m_rankScoreLabel->setText(rankScore + " RS");
        m_recordLabel->setText(record + " Qual Record"); });

    connect(reply, &QNetworkReply::errorOccurred, this, [=]
            { qDebug() << reply->error(); });
}

void EventData::updateEventData()
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://thebluealliance.com/api/v3/team/" + Constants::TEAM_NUMBER + "/event/" + Constants::EVENT_CODE + "/matches/simple"));
    request.setRawHeader("X-TBA-Auth-Key", Constants::AUTH_KEY);

    QNetworkReply *reply = m_manager->get(request);

    connect(reply, &QNetworkReply::readyRead, this, [this, reply]
            {
                const QByteArray data = reply->read(reply->bytesAvailable());

                const QJsonDocument json = QJsonDocument::fromJson(data);
                const QJsonArray jsonArray = json.array();

                // const QJsonValue match = latestMatch(jsonArray);
                const QJsonValue match = sortedMatches(jsonArray)[jsonArray.size() - 1];

                // Match Code
                const QString matchKey = match["key"].toString();

                m_lastMatchCode->setText("Last Match: " + matchKey);

                // Set team numbers
                setTeamNumbers(match, m_lastRedTeams, m_lastBlueTeams);
                setMatchScore(match, m_lastRedScore, m_lastBlueScore); });

    connect(reply, &QNetworkReply::errorOccurred, this, [=]
            { qDebug() << reply->error(); });
}