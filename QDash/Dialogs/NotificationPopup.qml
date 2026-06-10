// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later

import Carboxyl.Clover
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: notif

    property string level: NotificationHelper.level
    property string text: NotificationHelper.text
    property string title: NotificationHelper.title

    closePolicy: "NoAutoClose"
    height: NotificationHelper.height > 0 ? NotificationHelper.height : implicitContentHeight + 20
    padding: 10
    width: NotificationHelper.width

    background: Rectangle {
        id: back

        color: Clover.theme.alternateBase
        radius: 3

        MouseArea {
            anchors.fill: back

            onClicked: notif.close()
        }

        ProgressBar {
            id: prog

            clip: true
            height: 5
            value: 1.0

            background: Rectangle {
                color: Clover.theme.alternateBase
                implicitHeight: 5
                implicitWidth: notif.width
                radius: 10
            }
            contentItem: Item {
                implicitHeight: 5
                implicitWidth: notif.width

                Rectangle {
                    color: Clover.theme.currentAccent
                    height: parent.height
                    radius: 10
                    width: prog.visualPosition * parent.width
                }
            }

            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
        }
    }
    contentItem: RowLayout {
        Image {
            Layout.fillHeight: true
            Layout.preferredHeight: 35
            Layout.preferredWidth: 35
            fillMode: Image.PreserveAspectFit
            horizontalAlignment: Image.AlignHCenter
            source: "qrc:/" + notif.level
            verticalAlignment: Image.AlignVCenter
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 2
            color: Clover.theme.text
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.fillHeight: true
                leftPadding: 10
                rightPadding: 20
                text: title
                wrapMode: Text.WordWrap

                font {
                    bold: true
                    pixelSize: 16
                }
            }

            Label {
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.fillHeight: true
                Layout.fillWidth: true
                font.pixelSize: 14
                leftPadding: 10
                rightPadding: 20
                text: notif.text
                wrapMode: Text.WordWrap
            }
        }
    }
    enter: Transition {
        NumberAnimation {
            duration: 200
            from: 0.0
            property: "opacity"
            to: 1.0
        }
    }
    exit: Transition {
        NumberAnimation {
            duration: 200
            from: 1.0
            property: "opacity"
            to: 0.0
        }
    }

    Component.onCompleted: {
        NotificationHelper.onReady.connect(() => {
            progClose.stop()
            prog.value = 1.0
            open()
            progClose.start()
        })
    }
    onOpened: progClose.start()

    NumberAnimation {
        id: progClose

        duration: NotificationHelper.displayTime >= 0 ? NotificationHelper.displayTime : 0
        from: 1.0
        property: "value"
        target: prog
        to: 0.0

        onFinished: close()
    }
}
