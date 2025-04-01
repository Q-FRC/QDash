import QtQuick
import QtQuick.Controls.Material

import QFRCDashboard

AnimatedDialog {
    required property Item content
    id: config

    title: "Configure Widget"

    height: Math.min(
                window.height, Math.max(
                    implicitBackgroundHeight + topInset
                    + bottomInset, contentHeight + topPadding + bottomPadding
                    + (implicitHeaderHeight > 0 ? implicitHeaderHeight + spacing : 0) + (implicitFooterHeight > 0 ? implicitFooterHeight + spacing : 0)) + 75 * Constants.scalar)
    width: Math.min(window.width, 625 * Constants.scalar)

    standardButtons: Dialog.Ok | Dialog.Cancel

    ScrollView {
        clip: true

        contentWidth: width - 5 * Constants.scalar - effectiveScrollBarWidth

        anchors {
            fill: parent

            topMargin: 5 * Constants.scalar

            rightMargin: 5
        }

        contentChildren: [content]
    }

    function openDialog() {
        // TODO: This should be recursive but also idc
        for (var i = 0; i < layout.children.length; ++i) {
            var child = layout.children[i]
            if (typeof child !== "undefined" && "open" in child) {
                child.open()
            } else {
                for (var j = 0; j < child.children.length; ++j) {
                    let grandchild = child.children[j]

                    if (typeof grandchild !== "undefined"
                            && "open" in grandchild) {
                        grandchild.open()
                    }
                }
            }
        }

        open()
    }

    onAccepted: {
        // TODO: This should be recursive but also idc
        for (var i = 0; i < layout.children.length; ++i) {
            var child = layout.children[i]
            if (typeof child !== "undefined" && "accept" in child) {
                child.accept()
            } else {
                for (var j = 0; j < child.children.length; ++j) {
                    let grandchild = child.children[j]

                    if (typeof grandchild !== "undefined"
                            && "accept" in grandchild) {
                        grandchild.accept()
                    }
                }
            }
        }
    }
}
