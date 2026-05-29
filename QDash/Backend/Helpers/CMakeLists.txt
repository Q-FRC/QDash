# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

set(CMAKE_AUTOMOC ON)

qt_add_library(QDashHelpers STATIC
    CompileDefinitions.h
    NotificationHelper.cpp NotificationHelper.h
    PlatformHelper.cpp PlatformHelper.h
    FileSelect.h)

target_link_libraries(QDashHelpers PRIVATE
    Qt6::Core Qt6::Gui Qt6::Quick Qt6::Qml Qt6::Widgets)
