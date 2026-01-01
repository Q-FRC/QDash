// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtCore
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4
import QtQuick.Dialogs

import QDash.Constants
import QDash.Config

ColumnLayout {
    spacing: 5

    function accept() {
        theme.accept()
        accent.accept()

        Constants.setTheme(settings.theme)
        Constants.setAccent(settings.accent)
    }

    function open() {
        theme.open()
        accent.open()
    }

    RowLayout {
        Layout.fillWidth: true

        AppearanceComboBox {
            implicitHeight: 50
            implicitWidth: 200
            id: theme

            label: "Theme"

            bindedProperty: "theme"
            bindTarget: settings

            choices: ["Light", "Dark", "Midnight"]
        }

        AppearanceComboBox {
            implicitWidth: 200
            implicitHeight: 50
            id: accent

            label: "Accent"

            bindedProperty: "accent"
            bindTarget: settings

            choices: accents.names()
        }
    }
}
