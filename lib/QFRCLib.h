// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: BSD

#pragma once

#include <string>
#include <frc/TimedRobot.h>

namespace QFRCLib
{
  enum class ErrorLevel
  {
    Information,
    Warning,
    Critical
  };

  /**
   * A notification to be sent to the dashboard.
   */
  struct Notification
  {
    ErrorLevel level = ErrorLevel::Information;
    std::string title;
    std::string description;
    int displayTimeMillis = 5000;
    double width = 350.0;
    double height = -1.0;

    Notification() = default;
    Notification(ErrorLevel lvl, std::string t, std::string d)
        : level(lvl), title(std::move(t)), description(std::move(d)) {}

    Notification(ErrorLevel lvl, std::string t, std::string d, int displayMs,
                 double w, double h)
        : level(lvl), title(std::move(t)), description(std::move(d)),
          displayTimeMillis(displayMs), width(w), height(h) {}

    // chain setters
    Notification &withLevel(ErrorLevel l)
    {
      level = l;
      return *this;
    }
    Notification &withTitle(const std::string &t)
    {
      title = t;
      return *this;
    }
    Notification &withDescription(const std::string &d)
    {
      description = d;
      return *this;
    }
    Notification &withDisplayMilliseconds(int ms)
    {
      displayTimeMillis = ms;
      return *this;
    }
    Notification &withWidth(double w)
    {
      width = w;
      return *this;
    }
    Notification &withHeight(double h)
    {
      height = h;
      return *this;
    }
    Notification &withAutomaticHeight()
    {
      height = -1;
      return *this;
    }
  };

  // errors

  /**
   * Report an error to the dashboard.
   * @param level The error level.
   * @param message The error message.
   */
  void reportError(ErrorLevel level, const std::string &message);

  /**
   * Set the length of the error history.
   * @param length The maximum number of errors to keep in history.
   */
  void setErrorHistoryLength(size_t length);

  /**
   * Set the current tab in the dashboard.
   * @param tabName The name of the tab to set.
   */
  void setTab(const std::string &tabName);

  // misc

  /**
   * Send a {@link Notification} to the dashboard.
   * @param notification The notification to send.
   */
  void sendNotification(const Notification &notification);

  /**
   * Send a notification to the dashboard.
   * @param level The error level of the notification.
   * @param title The title of the notification.
   * @param description The description of the notification.
   * @param displayTimeMillis The time to display the notification in milliseconds.
   * @param width The width of the notification window.
   * @param height The height of the notification window. If -1, QDash will
   *      automatically set the height depending on the amount of content.
   */
  void sendNotification(ErrorLevel level, const std::string &title,
                        const std::string &description, int displayTimeMillis = 5000,
                        double width = 350.0, double height = -1.0);

  /**
   * Start a web server to serve files from the deploy directory.
   * The server will be accessible at port 5800.
   */
  void startWebServer();

  /**
   * Publish the match time to NetworkTables periodically.
   * @param robot The TimedRobot instance to add the periodic function to.
   */
  void publishMatchTime(frc::TimedRobot *robot);

} // namespace QFRCLib

