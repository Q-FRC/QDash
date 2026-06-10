// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import Carboxyl.Clover

import Carboxyl.Contour
import QtCore
import QtQuick
import QtQuick.Controls

CarboxylLabeledComboBox {
    id: control

    property list<var> choices

    function filter(filter) {
        let newList = []
        let regex = new RegExp(".*" + filter + ".*", "i")
        for (var i = 0; i < choices.length; ++i) {
            if (choices[i].match(regex)) {
                newList.push(choices[i])
            }
        }

        model = newList
    }

    model: choices

    popup: Popup {
        bottomMargin: 12
        height: Math.min(contentItem.implicitHeight + verticalPadding * 2, control.Window.height - topMargin - bottomMargin)
        topMargin: 12
        transformOrigin: Item.Top
        verticalPadding: 8
        width: control.width
        y: control.editable ? control.height - 5 : 0

        background: Rectangle {
            color: Clover.theme.base
            layer.enabled: control.enabled
            radius: 4
        }
        contentItem: Item {
            implicitHeight: search.height + 28 + listView.contentHeight
            width: parent.width

            CarboxylLabeledTextField {
                id: search

                font.pixelSize: 16
                label: "Search"

                onTextEdited: control.filter(text)

                anchors {
                    left: parent.left
                    margins: 10
                    right: parent.right
                    top: parent.top
                    topMargin: 18
                }
            }

            ListView {
                id: listView

                clip: true
                currentIndex: control.highlightedIndex
                highlightMoveDuration: 0
                model: control.delegateModel

                ScrollIndicator.vertical: ScrollIndicator {}

                anchors {
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                    top: search.bottom
                    topMargin: 10
                }
            }
        }
        enter: Transition {
            // grow_fade_in
            NumberAnimation {
                duration: 220
                easing.type: Easing.OutQuint
                from: 0.9
                property: "scale"
            }

            NumberAnimation {
                duration: 150
                easing.type: Easing.OutCubic
                from: 0.0
                property: "opacity"
            }
        }
        exit: Transition {
            // shrink_fade_out
            NumberAnimation {
                duration: 220
                easing.type: Easing.OutQuint
                property: "scale"
                to: 0.9
            }

            NumberAnimation {
                duration: 150
                easing.type: Easing.OutCubic
                property: "opacity"
                to: 0.0
            }
        }
    }

    onChoicesChanged: model = choices
}
