// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls

import Carboxyl.Clover

import Carboxyl.Contour

CarboxylDialog {
    id: config
    required property Item content

    popupType: Popup.Window

    implicitWidth: 550

    title: "Configure Widget"

    standardButtons: Dialog.Ok | Dialog.Cancel

    ScrollView {
        anchors {
            fill: parent
        }

        onWidthChanged: contentWidth = width - effectiveScrollBarWidth
        contentChildren: [content]
    }

    onAboutToShow: {
        // TODO: This should be recursive but also idc
        for (var i = 0; i < layout.children.length; ++i) {
            var child = layout.children[i];
            if (typeof child !== "undefined" && "open" in child) {
                child.open();
            } else {
                for (var j = 0; j < child.children.length; ++j) {
                    let grandchild = child.children[j];

                    if (typeof grandchild !== "undefined" && "open" in grandchild) {
                        grandchild.open();
                    }
                }
            }
        }
    }

    onAccepted: {
        // TODO: This should be recursive but also idc
        for (var i = 0; i < layout.children.length; ++i) {
            var child = layout.children[i];
            if (typeof child !== "undefined" && "accept" in child) {
                child.accept();
            } else {
                for (var j = 0; j < child.children.length; ++j) {
                    let grandchild = child.children[j];

                    if (typeof grandchild !== "undefined" && "accept" in grandchild) {
                        grandchild.accept();
                    }
                }
            }
        }

        twm.modified = true;
    }

    onClosed: if (configLoader)
        configLoader.active = false
}
