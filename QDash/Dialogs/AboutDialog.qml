// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls

import Carboxyl.Contour

MessageDialog {
    title: "About QDash"

    textFormat: Text.RichText

    width: 400
    height: 275

    text: "<p>QDash is a reliable, high-performance FRC dashboard with "
          + "a low resource cost, suited for low-end computers and for maximizing "
          + "Driver Station resources.</p>" + buildConfig.buildInfo(
              ) + "Copyleft 2023-2026 crueter"

    standardButtons: Dialog.Ok
}
