# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

set(CMAKE_AUTOMOC ON)

qt_add_library(QDashQmlDialogs STATIC)
qt6_add_qml_module(QDashQmlDialogs
    URI QDash.Dialogs
    VERSION 1.0
    QML_FILES
        AboutDialog.qml
        NotificationPopup.qml
        StyleWarnDialog.qml

        settings/AppearanceTab.qml
        settings/MiscTab.qml
        settings/ServerTab.qml
        settings/SettingsDialog.qml

        tab/TabCloseDialog.qml
        tab/TabDialog.qml
        tab/TabNameDialog.qml)

if (QDASH_NETWORK)
    qt_target_qml_sources(QDashQmlDialogs QML_FILES
        remote/real/RemoteLayout.qml
        remote/real/RemoteLayoutsDialog.qml)
else()
    qt_target_qml_sources(QDashQmlDialogs QML_FILES
        remote/null/RemoteLayoutsDialog.qml)
endif()
