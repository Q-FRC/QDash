import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

PrimitiveWidget {
    id: widget

    property int item_fontSize: 100
    property int item_decimals: 2
    property color item_color: Constants.accent

    Menu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Spin Box"
            onTriggered: {
                model.type = "double"
            }
        }

        MenuItem {
            text: "Dial"
            onTriggered: {
                model.type = "doubleDial"
            }
        }

        MenuItem {
            text: "Radial Gauge"
            onTriggered: {
                model.type = "doubleGauge"
            }
        }

        MenuItem {
            text: "Progress Bar"
            onTriggered: {
                model.type = "doubleBar"
            }
        }

        MenuItem {
            text: "Match Time"
            onTriggered: {
                model.type = "matchTime"
            }
        }
    }

    Component.onCompleted: rcMenu.addMenu(switchMenu)

    function update(value) {
        txt.value = value
    }

    Text {
        id: txt

        font.pixelSize: item_fontSize * Constants.scalar

        property double value

        text: value.toFixed(item_decimals)

        color: item_color
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        fontSizeMode: Text.Fit

        anchors {
            top: titleField.bottom
            right: parent.right
            left: parent.left
            bottom: parent.bottom

            margins: 10 * Constants.scalar
        }
    }

    BaseConfigDialog {
        id: config

        function openDialog() {
            topicField.open()
            titleFontField.open()
            fontField.open()

            decField.open()
            colorField.open()

            open()
        }

        onAccepted: {
            topicField.accept()
            titleFontField.accept()
            fontField.accept()
            decField.accept()

            colorField.accept()
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

                    label: "Maximum Font Size"

                    bindedProperty: "item_fontSize"
                    bindTarget: widget
                }
            }

            SectionHeader {
                label: "Display Settings"
            }

            RowLayout {
                uniformCellSizes: true

                LabeledSpinBox {
                    Layout.fillWidth: true

                    id: decField

                    label: "Number of Decimals"

                    bindedProperty: "item_decimals"
                    bindTarget: widget

                    from: 0
                }

                ColorField {
                    Layout.fillWidth: true

                    id: colorField

                    label: "Text Color"

                    bindedProperty: "item_color"
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
