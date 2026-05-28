# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

qt_add_library(QDashManagers STATIC
    ConnManager.cpp ConnManager.h
    SettingsManager.cpp SettingsManager.h)

target_link_libraries(QDashManagers PRIVATE ntcore Qt6::Core Qt6::Qml Qt6::Gui)
