import QtQuick
import QtQuick.Controls

import QFRCDashboard

AnimatedDialog {
    id: config

    title: "Configure Widget"

    width: 400 * Constants.scalar

    standardButtons: Dialog.Ok | Dialog.Cancel
}
