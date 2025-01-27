import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

BaseWidget {
    id: widget

    property string item_topic

    property int item_fontSize: 100

    Menu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Spin Box"
            onTriggered: {
                model.type = "int"
            }
        }

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
    }

    Component.onCompleted: {
        rcMenu.addMenu(switchMenu)
    }

    Text {
        id: txt

        font.pixelSize: item_fontSize * Constants.scalar

        property int value

        function updateTopic(ntTopic, ntValue) {
            if (ntTopic === item_topic) {
                value = ntValue
            }
        }

        text: value

        color: Constants.accent
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

        txt.updateTopic(item_topic, topicStore.getValue(item_topic))
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
            spacing: 25 * Constants.scalar

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.right

                topMargin: -20

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
