// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtCore
import QtQuick
import QtQuick.Controls

import Carboxyl.Clover
import Carboxyl.Contour

import QDash.Dialogs
import QDash.Components

Loader {
    id: loader
    active: false
    asynchronous: true
    onLoaded: item.open()

    function open() {
        active = true;
    }

    sourceComponent: active ? src : undefined

    property Component src: Component {
        CarboxylMessageDialog {
            text: qsTr("To apply the new style, QDash will now close and re-open.")
            icon: CarboxylEnums.Warning
            title: qsTr("Reloading")
            standardButtons: DialogButtonBox.Ok

            onClosed: QDashApplication.reload()
        }
    }
}
