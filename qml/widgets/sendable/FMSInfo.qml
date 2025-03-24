import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QFDFlags
import QFRCDashboard

BaseWidget {
    id: widget
    property string item_topic

    property int item_fontSize: 18

    ColumnLayout {
        anchors {
            top: titleField.bottom
            topMargin: 8

            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
        }

        // TODO: Don't show GSM if there is none
        Rectangle {
            Layout.fillWidth: true
            property bool isRedAlliance: false

            radius: 4 * Constants.scalar

            color: isRedAlliance ? "red" : "blue"

            implicitHeight: item_fontSize * 2 * Constants.scalar

            Text {
                anchors.fill: parent

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                color: Constants.palette.text

                property int matchNumber: 0
                property string matchType: "Unknown"
                property string eventName: "Event"
                property list<string> matchTypeMap: ["Unknown", "Practice", "Quals", "Elims"]

                id: match

                font.pixelSize: item_fontSize * Constants.scalar

                function updateTopic(ntTopic, value) {
                    if (ntTopic === item_topic + "/MatchNumber") {
                        matchNumber = value
                    } else if (ntTopic === item_topic + "/MatchType") {
                        matchType = matchTypeMap[value]
                    } else if (ntTopic === item_topic + "/EventName") {
                        eventName = value
                    } else if (ntTopic === item_topic + "/IsRedAlliance") {
                        parent.isRedAlliance = value
                    }
                }

                text: eventName + ": " + matchType + " Match " + matchNumber

                function update() {
                    topicStore.subscribe(item_topic + "/MatchNumber")
                    topicStore.subscribe(item_topic + "/MatchType")
                    topicStore.subscribe(item_topic + "/EventName")
                    topicStore.subscribe(item_topic + "/IsRedAlliance")
                }

                function unsubscribe() {
                    topicStore.unsubscribe(topic + "/MatchNumber")
                    topicStore.unsubscribe(topic + "/MatchType")
                    topicStore.unsubscribe(topic + "/EventName")
                    topicStore.unsubscribe(topic + "/IsRedAlliance")
                }

                Component.onCompleted: {
                    topicStore.topicUpdate.connect(updateTopic)
                    item_topic = model.topic

                    update()
                }

                Component.onDestruction: {
                    if (topicStore !== null) {
                        topicStore.topicUpdate.disconnect(updateTopic)
                        unsubscribe()
                    }
                }
            }
        }

        Text {
            Layout.fillWidth: true
            color: Constants.palette.text

            property string gameSpecificMessage: ""

            id: gsm

            font.pixelSize: item_fontSize * Constants.scalar

            function updateTopic(ntTopic, value) {
                if (ntTopic === item_topic + "/GameSpecificMessage") {
                    gameSpecificMessage = value
                }
            }

            horizontalAlignment: Text.AlignHCenter

            text: gameSpecificMessage

            function update() {
                topicStore.subscribe(item_topic + "/GameSpecificMessage")
            }

            function unsubscribe() {
                topicStore.unsubscribe(topic + "/GameSpecificMessage")
            }

            Component.onCompleted: {
                topicStore.topicUpdate.connect(updateTopic)
                item_topic = model.topic

                update()
            }
            Component.onDestruction: {
                if (topicStore !== null) {
                    topicStore.topicUpdate.disconnect(updateTopic)
                    unsubscribe()
                }
            }

            visible: gameSpecificMessage !== ""
        }

        Text {
            Layout.fillWidth: true
            color: Constants.palette.text

            property string state: "Unknown"

            id: stateText

            font.pixelSize: item_fontSize * Constants.scalar

            function updateTopic(ntTopic, value) {
                if (ntTopic === item_topic + "/FMSControlData") {
                    let word = topicStore.toWord(value)

                    state = ""

                    if (word & QFDFlags.Auto) {
                        state += "Autonomous"
                    } else if (word & QFDFlags.Test) {
                        state += "Testing"
                    } else {
                        state += "Teleop"
                    }

                    if (word & QFDFlags.Enabled) {
                        state += " Enabled"
                    } else if (word & QFDFlags.EStop) {
                        state += " E-Stopped"
                    } else {
                        state += " Disabled"
                    }
                }
            }

            horizontalAlignment: Text.AlignHCenter

            text: state

            // TODO: This is really jank
            // should have a single value descriptor in the root item
            // that handles everything
            function update() {
                topicStore.subscribe(item_topic + "/FMSControlData")

                topicStore.forceUpdate(item_topic + "/FMSControlData")
            }

            function unsubscribe() {
                topicStore.unsubscribe(topic + "/FMSControlData")
            }

            Component.onCompleted: {
                topicStore.topicUpdate.connect(updateTopic)
                item_topic = model.topic

                update()
            }
            Component.onDestruction: {
                if (topicStore !== null) {
                    topicStore.topicUpdate.disconnect(updateTopic)
                    unsubscribe()
                }
            }
        }
    }

    onItem_topicChanged: {
        match.unsubscribe()
        stateText.unsubscribe()
        gsm.unsubscribe()

        model.topic = item_topic

        match.update()
        stateText.update()
        gsm.update()
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
