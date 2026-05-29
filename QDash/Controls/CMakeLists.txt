# SPDX-FileCopyrightText: Copyright 2026 crueter
# SPDX-License-Identifier: GPL-3.0-or-later

qt_add_library(QDashQmlControls STATIC)
qt6_add_qml_module(QDashQmlControls
    URI QDash.Controls
    VERSION 1.0
    QML_FILES
        BaseConfigDialog.qml
        ColorField.qml
        LabeledCheckbox.qml
        LabeledComboBox.qml
        LabeledDoubleSpinBox.qml
        LabeledIndexComboBox.qml
        LabeledSpinBox.qml
        LabeledTextField.qml
        SectionHeader.qml)
