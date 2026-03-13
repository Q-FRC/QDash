// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import Carboxyl.Clover

PrimitiveWidget {
    id: widget

    property int fontSize: 100
    property int warningThreshold: 3
    property int flashInterval: 500

    propertyKeys: ["fontSize", "warningThreshold", "flashInterval"]

    readOnly: true

    readonly property color warningColor: "yellow"
    readonly property color activeColor: "#00c500"
    readonly property color inactiveColor: "red"

    enum Active {
        Red = 0,
        Blue = 1,
        Both = 2
    }

    property int active: PhaseShift.Both
    property int firstActive: PhaseShift.Red
    property int secondActive: PhaseShift.Blue
    property bool hubActive: active === PhaseShift.Both
                             || (redAlliance ? active === PhaseShift.Red : active
                                               === PhaseShift.Blue)
    property int remainingTime: 25
    property bool redAlliance: false
    property bool needsWarning: false
    property double rawTime: 130
    property int time: 130

    function updatePhase() {
        time = Math.ceil(rawTime)
        needsWarning = true
        // Transition Shift
        if (time > 130) {
            active = PhaseShift.Both
            remainingTime = time % 130
            // Phase 1
        } else if (time > 105) {
            active = firstActive
            remainingTime = time % 105
            // Phase 2
        } else if (time > 80) {
            active = secondActive
            remainingTime = time % 80
            // Phase 3
        } else if (time > 55) {
            active = firstActive
            remainingTime = time % 55
            // Phase 4
        } else if (time > 30) {
            active = secondActive
            remainingTime = time % 30
            // Endgame
        } else {
            needsWarning = false
            active = PhaseShift.Both
            remainingTime = time
        }
    }

    // absolute HACK
    onRemainingTimeChanged: {
        if (remainingTime < warningThreshold + 2) {
            warningTimer.start()
        } else if (remainingTime >= 20 || !needsWarning) {
            warningTimer.stop()
            txt.color = txt.currentColor
        } else {
            warningTimer.stop()
        }
    }

    function update(value) {
        rawTime = value

        updatePhase()
    }

    function updateRedAlliance(value) {
        redAlliance = value
    }

    function updateGSM(value) {
        switch (value) {
        case 'B':
            firstActive = PhaseShift.Red
            secondActive = PhaseShift.Blue
            break
        case 'R':
            firstActive = PhaseShift.Blue
            secondActive = PhaseShift.Red
            break
        default:
            break
        }

        updatePhase()
    }

    function unsubscribeFMSData() {
        if (TopicStore !== null) {
            TopicStore.unsubscribe("/FMSInfo/IsRedAlliance", updateRedAlliance)
            TopicStore.unsubscribe("/FMSInfo/GameSpecificMessage", updateGSM)
        }
    }

    function subscribeFMSData() {
        if (TopicStore !== null) {
            TopicStore.subscribe("/FMSInfo/IsRedAlliance", updateRedAlliance)
            TopicStore.subscribe("/FMSInfo/GameSpecificMessage", updateGSM)
        }
    }

    menuExtension: Component {
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
                text: "Number Display"
                onTriggered: {
                    model.type = "doubleDisplay"
                }
            }

            MenuItem {
                text: "Match Time"
                onTriggered: {
                    model.type = "matchTime"
                }
            }
        }
    }

    Component.onCompleted: subscribeFMSData()
    Component.onDestruction: unsubscribeFMSData()

    Text {
        id: txt

        font.pixelSize: fontSize

        property color currentColor: hubActive ? activeColor : inactiveColor
        text: remainingTime

        // 3 seconds before each shift, alternate yellow and current color
        Timer {
            id: warningTimer
            interval: flashInterval
            repeat: true
            triggeredOnStart: true
            running: false

            onTriggered: {
                if (remainingTime > warningThreshold || !needsWarning) {
                    txt.color = txt.currentColor
                } else if (txt.color === txt.currentColor) {
                    txt.color = widget.warningColor
                } else {
                    txt.color = txt.currentColor
                }
            }
        }

        color: currentColor

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        fontSizeMode: Text.Fit

        anchors {
            top: titleField.bottom
            right: parent.right
            left: parent.left
            bottom: parent.bottom

            margins: 10
        }
    }

    Loader {
        id: configLoader
        active: false
        asynchronous: true

        onLoaded: item.open()

        sourceComponent: BaseConfigDialog {
            id: config

            content: ColumnLayout {
                id: layout
                spacing: 12
                anchors.fill: parent
                anchors.leftMargin: 2
                clip: true

                SectionHeader {
                    label: "Font Settings"
                }

                RowLayout {
                    LabeledSpinBox {
                        Layout.fillWidth: true

                        label: qsTr("Title Font Size")
                        bindedProperty: "titleFontSize"
                    }

                    LabeledSpinBox {
                        Layout.fillWidth: true

                        label: qsTr("Maximum Font Size")
                        bindedProperty: "fontSize"
                    }
                }

                SectionHeader {
                    label: qsTr("Display Settings")
                }

                RowLayout {
                    LabeledSpinBox {
                        Layout.fillWidth: true

                        label: qsTr("Warning Threshold")
                        bindedProperty: "warningThreshold"

                        from: 0
                        to: 10
                    }

                    LabeledSpinBox {
                        Layout.fillWidth: true

                        label: qsTr("Warning Flash Interval")
                        bindedProperty: "flashInterval"
                    }
                }

                SectionHeader {
                    label: "NT Settings"
                }

                LabeledTextField {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop

                    label: qsTr("Topic")

                    bindedProperty: "item_topic"
                }
            }
        }
    }
}
