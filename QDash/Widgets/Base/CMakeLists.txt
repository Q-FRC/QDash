# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlWidgetsBase STATIC)
qt6_add_qml_module(QDashQmlWidgetsBase
    URI QDash.Widgets.Base
    VERSION 1.0
    QML_FILES
    BaseWidget.qml
    PrimitiveWidget.qml
    SendableWidget.qml
    ResizeComponent.qml)

QDashModule(QDashQmlWidgetsBase)
