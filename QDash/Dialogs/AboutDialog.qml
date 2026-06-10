// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Contour
import QtQuick
import QtQuick.Controls

Loader {
    id: loader

    property Component src: CarboxylMessageDialog {
        height: 275
        standardButtons: Dialog.Ok
        text: "<p>QDash is a reliable, high-performance FRC dashboard with " + "a low resource cost, suited for low-end computers and for maximizing " + "Driver Station resources.</p>" + buildConfig.buildInfo() + "Copyleft 2023-2026 crueter"
        textFormat: Text.RichText
        title: "About QDash"
        width: 400

        onClosed: loader.active = false
    }

    function open() {
        active = true
    }

    active: false
    asynchronous: true
    sourceComponent: active ? src : undefined

    onLoaded: item.open()
}
