import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

PrimitiveWidget {
    id: widget

    property int item_fontSize: 20

    property int item_stepSize: 1

    property int item_upperBound: 100000
    property int item_lowerBound: -100000

    Menu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Dial"
            onTriggered: {
                model.type = "dial"
            }
        }

        MenuItem {
            text: "Radial Gauge"
            onTriggered: {
                model.type = "gauge"
            }
        }

        MenuItem {
            text: "Number Display"
            onTriggered: {
                model.type = "intDisplay"
            }
        }
    }

    Component.onCompleted: rcMenu.addMenu(switchMenu)

    function update(value) {
        spin.value = value
    }

    BetterSpinBox {
        id: spin

        font.pixelSize: item_fontSize * Constants.scalar

        value: 0
        from: item_lowerBound
        to: item_upperBound
        stepSize: item_stepSize

        valid: widget.valid

        anchors {
            verticalCenter: parent.verticalCenter
            topMargin: titleField.height

            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
        }

        onValueModified: {
            widget.setValue(value)
        }
    }

    BaseConfigDialog {
        id: config

        function openDialog() {
            topicField.open()
            titleFontField.open()
            fontField.open()

            upField.open()
            lowField.open()
            stepField.open()

            open()
        }

        onAccepted: {
            topicField.accept()
            titleFontField.accept()
            fontField.accept()

            upField.accept()
            lowField.accept()
            stepField.accept()
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
                label: "Spin Box Settings"
            }

            RowLayout {
                uniformCellSizes: true

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: lowField

                    label: "Lower Bound"

                    bindedProperty: "item_lowerBound"
                    bindTarget: widget
                }

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: upField

                    label: "Upper Bound"

                    bindedProperty: "item_upperBound"
                    bindTarget: widget
                }
            }

            LabeledSpinBox {
                Layout.fillWidth: true

                id: stepField

                label: "Step Size"

                bindedProperty: "item_stepSize"
                bindTarget: widget

                from: 0
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
