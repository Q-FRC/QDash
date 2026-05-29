// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtCore
import QtQuick.Controls

import Carboxyl.Contour
import Carboxyl.Clover

CarboxylLabeledComboBox {
    property list<var> choices

    model: choices
    onChoicesChanged: model = choices

    id: control

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

    popup: Popup {
        y: control.editable ? control.height - 5 : 0
        width: control.width
        height: Math.min(contentItem.implicitHeight + verticalPadding * 2,
                         control.Window.height - topMargin - bottomMargin)
        transformOrigin: Item.Top
        topMargin: 12
        bottomMargin: 12
        verticalPadding: 8

        enter: Transition {
            // grow_fade_in
            NumberAnimation {
                property: "scale"
                from: 0.9
                easing.type: Easing.OutQuint
                duration: 220
            }
            NumberAnimation {
                property: "opacity"
                from: 0.0
                easing.type: Easing.OutCubic
                duration: 150
            }
        }

        exit: Transition {
            // shrink_fade_out
            NumberAnimation {
                property: "scale"
                to: 0.9
                easing.type: Easing.OutQuint
                duration: 220
            }
            NumberAnimation {
                property: "opacity"
                to: 0.0
                easing.type: Easing.OutCubic
                duration: 150
            }
        }

        contentItem: Item {
            implicitHeight: search.height + 28 + listView.contentHeight
            width: parent.width

            CarboxylLabeledTextField {
                id: search

                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right

                    margins: 10
                    topMargin: 18
                }

                font.pixelSize: 16

                label: "Search"

                onTextEdited: control.filter(text)
            }

            ListView {
                id: listView

                clip: true
                anchors {
                    top: search.bottom
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom

                    topMargin: 10
                }

                model: control.delegateModel
                currentIndex: control.highlightedIndex
                highlightMoveDuration: 0

                ScrollIndicator.vertical: ScrollIndicator {}
            }
        }

        background: Rectangle {
            radius: 4
            color: Clover.theme.base

            layer.enabled: control.enabled
        }
    }
}
