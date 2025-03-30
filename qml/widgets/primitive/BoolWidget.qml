import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

PrimitiveWidget {
    id: widget

    property int item_checkboxSize: 20

    Menu {
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

    CheckBox {
        id: control

        checked: false

        anchors {
            centerIn: parent
        }

        indicator.implicitHeight: item_checkboxSize * Constants.scalar
        indicator.implicitWidth: item_checkboxSize * Constants.scalar

        onToggled: widget.setValue(checked)
    }

    BaseConfigDialog {
        id: config

        function openDialog() {
            topicField.open()
            titleFontField.open()
            checkboxField.open()

            open()
        }

        onAccepted: {
            topicField.accept()
            titleFontField.accept()
            checkboxField.accept()
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
