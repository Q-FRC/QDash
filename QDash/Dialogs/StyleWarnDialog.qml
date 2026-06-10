// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import Carboxyl.Contour
import QDash.Components

import QDash.Dialogs
import QtCore
import QtQuick
import QtQuick.Controls

Loader {
    id: loader

    property Component src: Component {
        CarboxylMessageDialog {
            icon: CarboxylEnums.Warning
            standardButtons: DialogButtonBox.Ok
            text: qsTr("To apply the new style, QDash will now close and re-open.")
            title: qsTr("Reloading")

            onClosed: QDashApplication.reload()
        }
    }

    function open() {
        active = true
    }

    active: false
    asynchronous: true
    sourceComponent: active ? src : undefined

    onLoaded: item.open()
}
