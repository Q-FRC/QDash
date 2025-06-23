import QtQuick

import QDash

TextDialog {
    title: "About QDash"

    width: 325 * Constants.scalar
    height: 375 * Constants.scalar

    text: "<p>QDash is a reliable, high-performance FRC dashboard with "
          + "a low resource cost, suited for low-end computers and for maximizing "
          + "Driver Station resources.</p>" + buildConfig.buildInfo(
              ) + "Copyleft 2023-2025 Q-FRC and crueter"

    standardButtons: "Close"
}
