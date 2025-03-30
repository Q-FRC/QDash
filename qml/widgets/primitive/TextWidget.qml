import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

PrimitiveWidget {
    id: widget

    property int item_fontSize: 20

    Menu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Color"
            onTriggered: {
                model.type = "colorText"
            }
        }

        MenuItem {
            text: "Text Display"
            onTriggered: {
                model.type = "textDisplay"
            }
        }
    }

    Component.onCompleted: rcMenu.addMenu(switchMenu)

    function update(value) {
        textField.text = value
    }

    BetterTextField {
        id: textField

        font.pixelSize: item_fontSize * Constants.scalar

        text: ""
        valid: widget.valid

        anchors {
            verticalCenter: parent.verticalCenter
            topMargin: titleField.height

            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
        }

        onTextEdited: {
            widget.setValue(text)
        }
    }

    BaseConfigDialog {
        id: config

        function openDialog() {
            topicField.open()
            titleFontField.open()
            fontField.open()

            open()
        }

        onAccepted: {
            topicField.accept()
            titleFontField.accept()
            fontField.accept()
        }

        ColumnLayout {
            id: layout
            spacing: 12 * Constants.scalar

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.right

                topMargin: 5 * Constants.scalar

                rightMargin: 5
            }

            SectionHeader {
                label: "Font Settings"
            }

            RowLayout {
                uniformCellSizes: true

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: titleFontField

                    label: "Title Font Size"

                    bindedProperty: "item_titleFontSize"
                    bindTarget: widget
                }

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: fontField

                    label: "Font Size"

                    bindedProperty: "item_fontSize"
                    bindTarget: widget
                }
            }

            SectionHeader {
                label: "NT Settings"
            }

            LabeledTextField {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                id: topicField

                label: "Topic"

                bindedProperty: "item_topic"
                bindTarget: widget
            }
        }
    }
}
