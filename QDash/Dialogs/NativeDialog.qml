// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

import QDash.Constants

Dialog {
    popupType: Popup.Native
    anchors.centerIn: Overlay.overlay

    Overlay.modal: Item {}
    Overlay.modeless: Item {}

    enter: Transition {}

    exit: Transition {}

    background: Rectangle {
        color: Constants.palette.dialogBg
    }
}
