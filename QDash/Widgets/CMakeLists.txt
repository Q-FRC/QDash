# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

qt_add_library(QDashQmlWidgets STATIC)

qt_add_qml_module(QDashQmlWidgets
    URI QDash.Widgets
    VERSION 1.0
    QML_FILES
        base/BaseWidget.qml
        base/PrimitiveWidget.qml
        base/SendableWidget.qml
        base/ResizeAnchor.qml
        base/ResizeComponent.qml
        base/ShapeHandler.qml

        sendable/Command.qml
        sendable/FMSInfo.qml
        sendable/Field2d.qml
        sendable/StringChooser.qml

        bool/BoolWidget.qml
        bool/ColorWidget.qml

        string/ColorTextWidget.qml
        string/StringDisplay.qml
        string/TextWidget.qml

        double/DoubleDialWidget.qml
        double/DoubleDisplay.qml
        double/DoubleGaugeWidget.qml
        double/DoubleProgressBar.qml
        double/DoubleWidget.qml
        double/MatchTime.qml

        int/IntDialWidget.qml
        int/IntDisplay.qml
        int/IntWidget.qml)

if (QDASH_CAMVIEW)
    # TODO: Reduce duped code here
    qt_target_qml_sources(QDashQmlWidgets
        QML_FILES
            misc/CameraView.qml
            misc/UrlCameraView.qml)
endif()

if (QDASH_WEBVIEW)
    qt_target_qml_sources(QDashQmlWidgets
        QML_FILES
            misc/WebView.qml)
endif()
