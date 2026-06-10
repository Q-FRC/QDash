// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover

import QDash.Controls
import QDash.Widgets
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

SendableWidget {
    id: widget

    property int fontSize: 18

    function update(topic, value) {
        widget.connected = true
        switch (topic) {
        case "MatchNumber":
        {
            match.matchNumber = value
            break
        }
        case "MatchType":
        {
            match.matchType = match.matchTypeMap[value]
            break
        }
        case "EventName":
        {
            match.eventName = value === "" ? "Event" : value
            break
        }
        case "IsRedAlliance":
        {
            rect.isRedAlliance = value
            break
        }
        case "GameSpecificMessage":
        {
            gsm.gameSpecificMessage = value
            break
        }
        case "FMSControlData":
        {
            let state = QDashApplication.wordToState(value)
            stateText.state = state
            break
        }
        }
    }

    propertyKeys: ["fontSize"]
    topics: ["MatchNumber", "MatchType", "EventName", "IsRedAlliance", "GameSpecificMessage", "FMSControlData"]

    configContent: ColumnLayout {
        id: layout

        anchors.fill: parent
        anchors.leftMargin: 2
        clip: true
        spacing: 12

        SectionHeader {
            label: "Font Settings"
        }

        RowLayout {
            LabeledSpinBox {
                bindedProperty: "titleFontSize"
                label: "Title Font Size"
            }

            LabeledSpinBox {
                bindedProperty: "fontSize"
                label: "Font Size"
            }
        }

        SectionHeader {
            label: "NT Settings"
        }

        LabeledTextField {
            bindedProperty: "item_topic"
            label: "Topic"
        }
    }

    ColumnLayout {
        anchors {
            left: parent.left
            leftMargin: 10
            right: parent.right
            rightMargin: 10
            top: titleField.bottom
            topMargin: 8
        }

        Rectangle {
            id: rect

            property bool isRedAlliance: false

            Layout.fillWidth: true
            color: isRedAlliance ? "red" : "blue"
            implicitHeight: fontSize * 2
            radius: 4

            Label {
                id: match

                property string eventName: "Event"
                property int matchNumber: 0
                property string matchType: "Unknown"
                property list<string> matchTypeMap: ["Unknown", "Practice", "Quals", "Elims"]

                anchors.fill: parent
                enabled: widget.connected
                font.pixelSize: fontSize
                horizontalAlignment: Text.AlignHCenter
                text: eventName + ": " + matchType + " Match " + matchNumber
                verticalAlignment: Text.AlignVCenter
            }
        }

        Label {
            id: gsm

            property string gameSpecificMessage: ""

            Layout.fillWidth: true
            enabled: widget.connected
            font.pixelSize: fontSize
            horizontalAlignment: Text.AlignHCenter
            text: gameSpecificMessage
            visible: gameSpecificMessage !== ""
        }

        Label {
            id: stateText

            property string state: "Unknown"

            Layout.fillWidth: true
            enabled: widget.connected
            font.pixelSize: fontSize
            horizontalAlignment: Text.AlignHCenter
            text: state
        }
    }
}
