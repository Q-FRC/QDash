// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Config
import QDash.Items

RowLayout {
    spacing: 20

    function accept() {
        if (QDashSettings.style !== style.currentText) {
            QDashApplication.shouldReload = true
        }

        QDashSettings.style = style.currentText
        QDashSettings.theme = theme.currentIndex
        QDashSettings.accent = accent.currentIndex

        fontSize.accept()
        displayFontSize.accept()
        titleFontSize.accept()

        Clover.accent = Clover.accents[accent.currentIndex]
        Clover.theme = Clover.themes[theme.currentIndex]

        // TODO(crueter): Better handling of background images and such
        // This is just bespoke for hannah montana right now
        if (Clover.theme === CustomThemes.hannahDark) {
            QDashSettings.hannahMontanaMode = true
            console.log("HANNAH MONTANAAAAAAA")

            // FIXME(crueter): Automatically set violet accent? Needs Carboxyl option perhaps
        }
    }

    ColumnLayout {
        spacing: 20
        CarboxylLabeledComboBox {
            id: style

            model: CarboxylConfig.styles

            implicitHeight: 45
            Layout.fillWidth: true
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
            Layout.fillWidth: true
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
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            font.pixelSize: 20

            Component.onCompleted: currentIndex = QDashSettings.theme
        }
    }

    ColumnLayout {
        spacing: 20
        LabeledSpinBox {
            id: fontSize

            label: "Default Field Font Size"
            bindTarget: QDashSettings
            bindedProperty: "defaultFontSize"

            implicitHeight: 45
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            font.pixelSize: 20

            // TODO(crueter): This can probably be moved to the ConfigSpinBox because of loaders
            Component.onCompleted: open()
        }

        LabeledSpinBox {
            id: displayFontSize

            label: "Default Display Font Size"
            bindTarget: QDashSettings
            bindedProperty: "defaultDisplayFontSize"

            implicitHeight: 45
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            font.pixelSize: 20

            // TODO(crueter): This can probably be moved to the ConfigSpinBox because of loaders
            Component.onCompleted: open()
        }

        LabeledSpinBox {
            id: titleFontSize

            label: "Default Title Font Size"
            bindTarget: QDashSettings
            bindedProperty: "defaultTitleFontSize"

            implicitHeight: 45
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            font.pixelSize: 20

            // TODO(crueter): This can probably be moved to the ConfigSpinBox because of loaders
            Component.onCompleted: open()
        }
    }
}
