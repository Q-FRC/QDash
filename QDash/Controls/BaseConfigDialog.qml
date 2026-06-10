// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Core

import QtQuick
import QtQuick.Controls

CarboxylDialog {
    id: config

    required property Component configComponent

    implicitWidth: 550
    popupType: Popup.Window
    standardButtons: Dialog.Ok | Dialog.Cancel
    title: "Configure Widget"

    onAccepted: {
        Util.searchFunction(content.item, "accept")
        twm.modified = true
    }

    ScrollView {
        id: scroll
        contentChildren: [content.item]
        onWidthChanged: contentWidth = width - effectiveScrollBarWidth
        anchors.fill: parent

        Loader {
            id: content

            asynchronous: true
            sourceComponent: configComponent

            onLoaded: Util.searchFunction(item, "open")
        }
    }
}
