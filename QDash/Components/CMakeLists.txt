# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

qt_add_library(QDashQmlComponents STATIC)
qt6_add_qml_module(QDashQmlComponents
    URI QDash.Components
    VERSION 1.0
    QML_FILES
        MainScreen.qml
        Tab.qml
        TopicView.qml
        SearchableComboBox.qml)
