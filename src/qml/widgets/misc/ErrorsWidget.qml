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
