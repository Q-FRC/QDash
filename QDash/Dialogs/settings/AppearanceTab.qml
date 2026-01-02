// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtCore
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4
import QtQuick.Dialogs

import Carboxyl.Clover
import QDash.Config

import Carboxyl.Contour
import Carboxyl.Clover

ColumnLayout {
    spacing: 5

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
        implicitWidth: 350
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        label: "Style"

        Component.onCompleted: currentIndex = model.indexOf(
                                   CarboxylApplication.styleName)
    }

    CarboxylLabeledComboBox {
        id: accent

        model: Clover.accents
        textRole: "name"

        label: "Accent"

        implicitHeight: 45
        implicitWidth: 350
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        Component.onCompleted: currentIndex = QDashSettings.accent
    }

    CarboxylLabeledComboBox {
        id: theme

        model: Clover.themes
        textRole: "name"

        label: "Theme"

        implicitHeight: 45
        implicitWidth: 350
        Layout.alignment: Qt.AlignCenter
        font.pixelSize: 20

        Component.onCompleted: currentIndex = QDashSettings.theme
    }
}
