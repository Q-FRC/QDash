import QtQuick
import QtQuick.Controls.Material

import QFRCDashboard

AnimatedDialog {
    id: config

    title: "Configure Widget"

    height: Math.min(
                window.height, Math.max(
                    implicitBackgroundHeight + topInset
                    + bottomInset, contentHeight + topPadding + bottomPadding
                    + (implicitHeaderHeight > 0 ? implicitHeaderHeight + spacing : 0) + (implicitFooterHeight > 0 ? implicitFooterHeight + spacing : 0)) + 50 * Constants.scalar)
    width: Math.min(window.width, 625 * Constants.scalar)

    standardButtons: Dialog.Ok | Dialog.Cancel
}
