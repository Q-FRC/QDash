import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import QFRCDashboard

Popup {
    id: notif
    padding: 10

    closePolicy: "NoAutoClose"

    width: notificationHelper.width
    height: notificationHelper.height > 0 ? notificationHelper.height : implicitContentHeight + 20

    property string level: notificationHelper.level
    property string title: notificationHelper.title
    property string text: notificationHelper.text

    enter: Transition {
        NumberAnimation {
            property: "opacity"
            duration: 200

            from: 0.0
            to: 1.0
        }
    }

    exit: Transition {
        NumberAnimation {
            property: "opacity"
            duration: 200

            from: 1.0
            to: 0.0
        }
    }

    NumberAnimation {
        id: progClose
        target: prog
        property: "value"
        duration: notificationHelper.displayTime

        from: 1.0
        to: 0.0

        onFinished: close()
    }

    onOpened: progClose.start()
    Component.onCompleted: notificationHelper.onReady.connect(() => {
                                                                  progClose.stop()
                                                                  prog.value = 1.0
                                                                  open()
                                                                  progClose.start()
                                                              });

    background: Rectangle {
        id: back

        topLeftRadius: 10
        topRightRadius: 10

        color: Constants.palette.dialogBg

        MouseArea {
            anchors.fill: back
            onClicked: notif.close()
        }

        ProgressBar {
            id: prog
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            height: 5

            value: 1.0

            clip: true

            background: Rectangle {
                radius: 10
                color: Constants.palette.dialogBg
                implicitWidth: notif.width
                implicitHeight: 5
            }

            contentItem: Item {
                implicitWidth: notif.width
                implicitHeight: 5

                Rectangle {
                    width: prog.visualPosition * parent.width
                    height: parent.height
                    radius: 10
                    color: Constants.accent
                }
            }
        }
    }

    contentItem: RowLayout {
        Image {
            Layout.fillHeight: true
            verticalAlignment: Image.AlignVCenter
            horizontalAlignment: Image.AlignHCenter

            Layout.preferredWidth: 35
            Layout.preferredHeight: 35

            fillMode: Image.PreserveAspectFit

            source: "qrc:/" + notif.level
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 2

            color: Constants.palette.text
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text {
                Layout.fillHeight: true
                font {
                    pixelSize: 16
                    bold: true
                }

                color: Constants.palette.text

                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                leftPadding: 10
                rightPadding: 20

                text: title
                wrapMode: Text.WordWrap
            }

            Text {
                Layout.fillHeight: true
                Layout.fillWidth: true

                font.pixelSize: 14

                color: Constants.palette.text

                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                leftPadding: 10
                rightPadding: 20

                text: notif.text
                wrapMode: Text.WordWrap
            }
        }
    }
}
