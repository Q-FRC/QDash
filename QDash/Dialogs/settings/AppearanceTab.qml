// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4

import Carboxyl.Clover
import QDash.Config

import Carboxyl.Contour

ColumnLayout {
    spacing: 15

    readonly property int controlWidth: Math.min(width, 350)

    function accept() {
        if (QDashSettings.style !== style.currentText) {
            QDashApplication.shouldReload = true
        }

        QDashSettings.style = style.currentText
        QDashSettings.theme = theme.currentIndex
        QDashSettings.accent = accent.currentIndex

        Clover.accent = Clover.accents[accent.currentIndex]
        Clover.theme = Clover.themes[theme.currentIndex]
    }

    CarboxylLabeledComboBox {
        id: style

        model: CarboxylConfig.styles

        implicitHeight: 45
        implicitWidth: controlWidth
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        label: qsTr("Style")

        Component.onCompleted: currentIndex = model.indexOf(
                                   CarboxylApplication.styleName)
    }

    CarboxylLabeledComboBox {
        id: accent

        model: Clover.accents
        textRole: "name"

        label: qsTr("Accent")

        implicitHeight: 45
        implicitWidth: controlWidth
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        Component.onCompleted: currentIndex = QDashSettings.accent
    }

    CarboxylLabeledComboBox {
        id: theme

        model: Clover.themes
        textRole: "name"

        label: qsTr("Theme")

        implicitHeight: 45
        implicitWidth: controlWidth
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        Component.onCompleted: currentIndex = QDashSettings.theme
    }
}
