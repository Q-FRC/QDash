// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: BSD

#pragma once

#include <string>
#include <vector>

namespace QFRCLib {
enum class ErrorLevel { Information, Warning, Critical };

struct Notification {
  ErrorLevel level = ErrorLevel::Information;
  std::string title;
  std::string description;
  int displayTimeMillis = 3000;
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
  Notification& withLevel(ErrorLevel l) { level = l; return *this; }
  Notification& withTitle(const std::string& t) { title = t; return *this; }
  Notification& withDescription(const std::string& d) { description = d; return *this; }
  Notification& withDisplayMilliseconds(int ms) { displayTimeMillis = ms; return *this; }
  Notification& withWidth(double w) { width = w; return *this; }
  Notification& withHeight(double h) { height = h; return *this; }
  Notification& withAutomaticHeight() { height = -1; return *this; }
};

// errors
void reportError(ErrorLevel level, const std::string& message);
void setErrorHistoryLength(int length);
void setTab(const std::string& tabName);

// misc
void sendNotification(const Notification& notification);
void sendNotification(ErrorLevel level, const std::string& title,
                      const std::string& description, int displayTimeMillis = 3000,
                      double width = 350.0, double height = -1.0);
void startWebServer();

} // namespace QFRCLib
