// SPDX-FileCopyrightText: Copyright 2025 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls

import QDash.Constants
import QDash.Dialogs

NativeDialog {
    required property Item content
    id: config

    width: 550

    title: "Configure Widget"

    standardButtons: Dialog.Ok | Dialog.Cancel

    ScrollView {
        clip: true

        anchors {
            fill: parent
        }

        onWidthChanged: contentWidth = width - effectiveScrollBarWidth

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
