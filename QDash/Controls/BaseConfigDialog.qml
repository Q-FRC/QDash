// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Core

import QtQuick
import QtQuick.Controls

CarboxylDialog {
    id: config

    required property Item content

    implicitWidth: 550
    popupType: Popup.Window
    standardButtons: Dialog.Ok | Dialog.Cancel
    title: "Configure Widget"

    onAboutToShow: Util.searchFunction(content, "open")
    onAccepted: {
        Util.searchFunction(content, "accept")
        twm.modified = true
    }

    ScrollView {
        contentChildren: [content]

        onWidthChanged: contentWidth = width - effectiveScrollBarWidth

        anchors {
            fill: parent
        }
    }
}
