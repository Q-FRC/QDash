// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import Carboxyl.Contour
import QtQuick
import QtQuick.Controls

Loader {
    id: loader

    property Component src: CarboxylMessageDialog {
        icon: CarboxylEnums.Question
        standardButtons: Dialog.Yes | Dialog.No
        text: "Are you sure you want to close this tab?"
        textFont.pixelSize: 16
        title: "Close Tab?"

        onAccepted: loader.accepted()
        onClosed: {
            loader.active = false
        }
    }

    signal accepted

    function open() {
        active = true
    }

    active: false
    asynchronous: true
    sourceComponent: active ? src : undefined

    onLoaded: item.open()
}
