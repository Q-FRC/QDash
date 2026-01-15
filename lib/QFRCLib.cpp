
// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: BSD

#include "QFRCLib.h"

#include <deque>
#include <memory>
#include <mutex>
#include <vector>

#include <wpi/json.h>
#include <wpi/print.h>
#include <wpinet/WebServer.h>
#include <frc/Filesystem.h>
#include <frc/DriverStation.h>
#include <networktables/NetworkTableInstance.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/StringTopic.h>
#include <networktables/DoubleTopic.h>

using namespace nt;

namespace QFRCLib
{

  struct Error
  {
    std::string message;
    ErrorLevel level;
    Error(const std::string &m, ErrorLevel l) : message(m), level(l) {}
  };

  // "who needs state when you have static"
  static NetworkTableInstance inst = NetworkTableInstance::GetDefault();
  static std::shared_ptr<NetworkTable> table = inst.GetTable("QDash");
  static NetworkTableEntry tabEntry = table->GetEntry("Tab");
  static NetworkTableEntry errorsEntry = table->GetEntry("Errors");

  static std::deque<Error> errorsDeque;
  static size_t errorHistoryLength = 5;

  // error publishing is locked behind a mutex to prevent networktables from committing suicide
  static std::mutex g_mutex;

  static StringTopic notificationsTopic =
      NetworkTableInstance::GetDefault().GetStringTopic("/QDash/RobotNotifications");
  static StringPublisher notificationsPublisher = notificationsTopic.Publish();

  static DoubleTopic matchTimeTopic = NetworkTableInstance::GetDefault().GetDoubleTopic("/QDash/Match Time");
  static DoublePublisher matchTimePublisher = matchTimeTopic.Publish();

  // c++ doesn't have reflection
  // epic
  static const char *ErrorLevelToString(ErrorLevel lvl)
  {
    switch (lvl)
    {
    case ErrorLevel::Information:
      return "Information";
    case ErrorLevel::Warning:
      return "Warning";
    case ErrorLevel::Critical:
      return "Critical";
    default:
      return "Information";
    }
  }

  void publishErrors()
  {
    std::lock_guard<std::mutex> lk(g_mutex);
    std::vector<std::string> arr;
    arr.reserve(errorsDeque.size() * 2);
    for (const auto &e : errorsDeque)
    {
      arr.push_back(ErrorLevelToString(e.level));
      arr.push_back(e.message);
    }
    errorsEntry.SetStringArray(arr);
  }

  void reportError(ErrorLevel level, const std::string &message)
  {
    {
      std::lock_guard<std::mutex> lk(g_mutex);
      errorsDeque.emplace_back(message, level);
      while (errorsDeque.size() > errorHistoryLength)
      {
        errorsDeque.pop_front();
      }
    }
    publishErrors();
  }

  void setErrorHistoryLength(size_t length)
  {
    std::lock_guard<std::mutex> lk(g_mutex);
    errorHistoryLength = length;
    while (errorsDeque.size() > errorHistoryLength)
    {
      errorsDeque.pop_front();
    }
  }

  void setTab(const std::string &tabName)
  {
    tabEntry.SetString(tabName);
  }

  void startWebServer()
  {
    static const auto dir = frc::filesystem::GetDeployDirectory();
    wpi::WebServer::GetInstance().Start(5800, dir);
  }

  void publishMatchTime(frc::TimedRobot *robot)
  {
    robot->AddPeriodic(
        []()
        {
          double time = frc::DriverStation::GetMatchTime().value();
          matchTimePublisher.Set(time);
        },
        100_ms);
  }

  void sendNotification(const Notification &notification)
  {
    wpi::json j;
    j["level"] = ErrorLevelToString(notification.level);
    j["title"] = notification.title;
    j["description"] = notification.description;
    j["displayTime"] = notification.displayTimeMillis;
    j["width"] = notification.width;
    j["height"] = notification.height;

    try
    {
      std::string s = j.dump();
      notificationsPublisher.Set(s);
    }
    catch (...)
    {
      // HNNNNNG
    }
  }

  void sendNotification(ErrorLevel level, const std::string &title,
                        const std::string &description, int displayTimeMillis,
                        double width, double height)
  {
    Notification n;
    n.level = level;
    n.title = title;
    n.description = description;
    n.displayTimeMillis = displayTimeMillis;
    n.width = width;
    n.height = height;
    sendNotification(n);
  }

} // namespace QFRCLib
