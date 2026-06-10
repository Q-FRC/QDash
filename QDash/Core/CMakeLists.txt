# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

set_source_files_properties(Util.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_library(QDashQmlCore STATIC)
qt6_add_qml_module(QDashQmlCore
    URI QDash.Core
    VERSION 1.0
    QML_FILES
    IconButton.qml
    RadialGauge.qml
    DoubleSpinBox.qml
    AcceleratedShape.qml
    Util.qml)
