// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Controls
import QDash.Main
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.8

RowLayout {
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
        Clover.theme = Clover.themes[theme.currentIndex];

        // TODO(crueter): Better handling of background images and such
        // This is just bespoke for hannah montana right now
        if (Clover.theme === CustomThemes.hannahDark) {
            QDashSettings.hannahMontanaMode = true;
            // FIXME(crueter): Automatically set violet accent? Needs Carboxyl option perhaps
        } else {
            QDashSettings.hannahMontanaMode = false
        }
    }

    spacing: 20

    ColumnLayout {
        spacing: 20

        CarboxylLabeledComboBox {
            id: style

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            font.pixelSize: 20
            implicitHeight: 45
            label: qsTr("Style")
            model: CarboxylConfig.styles

            Component.onCompleted: currentIndex = model.indexOf(CarboxylApplication.styleName)
        }

        CarboxylLabeledComboBox {
            id: accent

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            font.pixelSize: 20
            implicitHeight: 45
            label: qsTr("Accent")
            model: Clover.accents
            textRole: "name"

            Component.onCompleted: currentIndex = QDashSettings.accent
        }

        CarboxylLabeledComboBox {
            id: theme

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            font.pixelSize: 20
            implicitHeight: 45
            label: qsTr("Theme")
            model: Clover.themes
            textRole: "name"

            Component.onCompleted: currentIndex = QDashSettings.theme
        }
    }

    ColumnLayout {
        spacing: 20

        LabeledSpinBox {
            id: fontSize

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            bindTarget: QDashSettings
            bindedProperty: "defaultFontSize"
            font.pixelSize: 20
            implicitHeight: 45
            label: "Default Field Font Size"

            // TODO(crueter): This can probably be moved to the ConfigSpinBox because of loaders
            Component.onCompleted: open()
        }

        LabeledSpinBox {
            id: displayFontSize

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            bindTarget: QDashSettings
            bindedProperty: "defaultDisplayFontSize"
            font.pixelSize: 20
            implicitHeight: 45
            label: "Default Display Font Size"

            // TODO(crueter): This can probably be moved to the ConfigSpinBox because of loaders
            Component.onCompleted: open()
        }

        LabeledSpinBox {
            id: titleFontSize

            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            bindTarget: QDashSettings
            bindedProperty: "defaultTitleFontSize"
            font.pixelSize: 20
            implicitHeight: 45
            label: "Default Title Font Size"

            // TODO(crueter): This can probably be moved to the ConfigSpinBox because of loaders
            Component.onCompleted: open()
        }
    }
}
