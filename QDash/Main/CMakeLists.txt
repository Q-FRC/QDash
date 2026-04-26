# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlMain STATIC)
qt6_add_qml_module(QDashQmlMain
    URI QDash.Main
    VERSION 1.0
    QML_FILES
        Main.qml
    RESOURCES
        hannahmontana.jpg
    RESOURCE_PREFIX "/qt/qml")

QDashModule(QDashQmlMain)
