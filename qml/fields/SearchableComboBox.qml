import QtQuick
import QtCore
import QtQuick.Controls.Material
import QFRCDashboard

BetterComboBox {
    property list<string> choices

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

        Material.theme: control.Material.theme
        Material.accent: control.Material.accent
        Material.primary: control.Material.primary

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
            implicitHeight: search.height + 28 * Constants.scalar + listView.contentHeight
            width: parent.width

            BetterTextField {
                id: search

                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right

                    margins: 10 * Constants.scalar
                    topMargin: 18 * Constants.scalar
                }

                font.pixelSize: 16 * Constants.scalar

                placeholderText: "Search"

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

                    topMargin: 10 * Constants.scalar
                }

                model: control.delegateModel
                currentIndex: control.highlightedIndex
                highlightMoveDuration: 0

                ScrollIndicator.vertical: ScrollIndicator {}
            }
        }

        background: Rectangle {
            radius: 4
            color: parent.Material.dialogColor

            layer.enabled: control.enabled
        }
    }
}
