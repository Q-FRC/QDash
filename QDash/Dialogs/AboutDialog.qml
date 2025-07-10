import QtQuick

import QDash.Constants

TextDialog {
    title: "About QDash"

    width: 325
    height: 375

    text: "<p>QDash is a reliable, high-performance FRC dashboard with "
          + "a low resource cost, suited for low-end computers and for maximizing "
          + "Driver Station resources.</p>" + buildConfig.buildInfo(
              ) + "Copyleft 2023-2025 Q-FRC and crueter"

    standardButtons: "Close"
}
