import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts
 
import fields
import items
import config
import basewidget
import constants


PrimitiveWidget {
    id: widget

    property int item_checkboxSize: 20

    BetterMenu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Color Display"
            onTriggered: {
                model.type = "color"
            }
        }
    }

    Component.onCompleted: rcMenu.addMenu(switchMenu)

    function update(value) {
        control.checked = value
    }

    Item {
        anchors {
            top: titleField.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
        }

        CheckBox {
            id: control

            checked: false

            enabled: widget.connected

            anchors {
                centerIn: parent
            }

            indicator.implicitHeight: item_checkboxSize * Constants.scalar
            indicator.implicitWidth: item_checkboxSize * Constants.scalar

            onToggled: widget.setValue(checked)
        }
    }

    BaseConfigDialog {
        id: config

        content: ColumnLayout {
            id: layout
            spacing: 12 * Constants.scalar
            anchors.fill: parent
            anchors.leftMargin: 2
            clip: true

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

                    id: checkboxField

                    label: "Checkbox Size"

                    bindedProperty: "item_checkboxSize"
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
