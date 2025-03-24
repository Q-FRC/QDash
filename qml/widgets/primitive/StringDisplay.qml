import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

BaseWidget {
    id: widget

    property string item_topic

    property int item_fontSize: 100
    property color item_color: Constants.accent
    property bool item_wrap: true

    Menu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Text Field"
            onTriggered: {
                model.type = "string"
            }
        }

        MenuItem {
            text: "Color"
            onTriggered: {
                model.type = "colorText"
            }
        }
    }

    Component.onCompleted: {
        rcMenu.addMenu(switchMenu)
    }

    Text {
        id: txt

        font.pixelSize: item_fontSize * Constants.scalar

        property string value

        function updateTopic(ntTopic, ntValue) {
            if (ntTopic === item_topic) {
                value = ntValue
            }
        }

        text: value

        color: item_color
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        fontSizeMode: Text.Fit

        wrapMode: item_wrap ? Text.WrapAtWordBoundaryOrAnywhere : Text.NoWrap

        anchors {
            top: titleField.bottom
            right: parent.right
            left: parent.left
            bottom: parent.bottom

            margins: 10 * Constants.scalar
        }

        Component.onCompleted: {
            topicStore.topicUpdate.connect(updateTopic)
            item_topic = model.topic
        }

        Component.onDestruction: {
            if (topicStore !== null) {
                topicStore.topicUpdate.disconnect(updateTopic)
                topicStore.unsubscribe(item_topic)
            }
        }
    }

    onItem_topicChanged: {
        topicStore.unsubscribe(topic)
        topicStore.subscribe(item_topic)
        model.topic = item_topic

        topicStore.forceUpdate(item_topic)
    }

    BaseConfigDialog {
        id: config

        function openDialog() {
            topicField.open()
            titleFontField.open()
            fontField.open()
            colorField.open()
            wrapField.open()

            open()
        }

        onAccepted: {
            topicField.accept()
            titleFontField.accept()
            fontField.accept()
            colorField.accept()
            wrapField.accept()
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

                ColorField {
                    Layout.fillWidth: true

                    id: colorField

                    label: "Text Color"

                    bindedProperty: "item_color"
                    bindTarget: widget
                }

                LabeledCheckbox {
                    Layout.fillWidth: true

                    id: wrapField

                    label: "Wrap Text?"

                    bindedProperty: "item_wrap"
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
