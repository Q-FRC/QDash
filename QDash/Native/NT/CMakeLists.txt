# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

qt_add_library(QDashNt STATIC
    TopicStore.cpp
    TopicStore.h)

target_link_libraries(QDashNt PRIVATE ntcore Qt6::Core Qt6::Qml)
