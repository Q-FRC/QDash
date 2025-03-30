import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

PrimitiveWidget {
    id: widget

    property int item_fontSize: 20

    property list<string> errors

    function update(value) {
        errors = value
    }

    ListView {
        id: listView

        interactive: false
        clip: true

        model: errors.length / 2

        delegate: Row {
            required property int modelData

            spacing: 5
            Image {
                source: "qrc:/" + errors[modelData * 2]
                width: item_fontSize * Constants.scalar
                height: item_fontSize * Constants.scalar
            }

            Text {
                font.pixelSize: item_fontSize * Constants.scalar
                color: Constants.palette.text
                text: errors[modelData * 2 + 1]
                wrapMode: Text.WrapAnywhere
            }
        }

        boundsBehavior: Flickable.StopAtBounds

        anchors {
            top: titleField.bottom
            bottom: parent.bottom

            left: parent.left
            right: parent.right

            margins: 10 * Constants.scalar
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

                rightMargin: 5 * Constants.scalar
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

                LabeledDoubleSpinBox {
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
