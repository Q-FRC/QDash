# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlWidgetsMisc STATIC)
qt6_add_qml_module(QDashQmlWidgetsMisc
    URI QDash.Widgets.Misc
    VERSION 1.0
    QML_FILES
    CameraView.qml
    ErrorsWidget.qml
    WebView.qml
    QML_FILES UrlCameraView.qml)

QDashModule(QDashQmlWidgetsMisc)
