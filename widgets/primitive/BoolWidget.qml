import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFRCDashboard

BaseWidget {
    id: widget

    property string item_topic

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

    Component.onCompleted: {
        rcMenu.addMenu(switchMenu)
    }

    CheckBox {
        id: control

        function updateTopic(ntTopic, ntValue) {
            if (ntTopic === item_topic) {
                checked = ntValue
            }
        }

        checked: false

        anchors {
            centerIn: parent
        }

        indicator.implicitHeight: item_checkboxSize * Constants.scalar
        indicator.implicitWidth: item_checkboxSize * Constants.scalar

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

        onToggled: topicStore.setValue(item_topic, checked)
    }

    onItem_topicChanged: {
        topicStore.unsubscribe(topic)
        topicStore.subscribe(item_topic)
        model.topic = item_topic

        control.checked = topicStore.getValue(item_topic)
    }

    BaseConfigDialog {
        id: config

        height: 325 * Constants.scalar

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
