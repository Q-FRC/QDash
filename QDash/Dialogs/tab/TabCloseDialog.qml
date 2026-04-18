// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls

import Carboxyl.Clover
import Carboxyl.Contour

MessageDialog {
    title: "Close Tab?"

    text: "Are you sure you want to close this tab?"
    icon: CarboxylEnums.Question
    textFont.pixelSize: 16

    standardButtons: Dialog.Yes | Dialog.No
}
