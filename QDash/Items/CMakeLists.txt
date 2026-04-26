# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

set(CMAKE_AUTOMOC ON)

set_source_files_properties(CustomThemes.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_library(QDashQmlItems STATIC)
qt6_add_qml_module(QDashQmlItems
    URI QDash.Items
    VERSION 1.0
    QML_FILES
    IconButton.qml
    MainScreen.qml
    RadialGauge.qml
    Tab.qml
    TopicView.qml
    ResizeAnchor.qml
    DoubleSpinBox.qml
    AcceleratedShape.qml
    CustomThemes.qml)

QDashModule(QDashQmlItems)
