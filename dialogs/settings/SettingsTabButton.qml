import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QFRCDashboard

TabButton {
    required property string label

    id: button

    height: 100

    contentItem: ColumnLayout {
        Button {
            Layout.maximumHeight: 75
            Layout.maximumWidth: 75

            id: image
            icon.source: "qrc:/" + label
            icon.width: 45
            icon.height: 45
            icon.color: Constants.accent

            background: Item {}

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            onClicked: button.click()
        }

        Label {
            font.pixelSize: 18
            text: label

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            color: Constants.palette.text
        }
    }

    background: Rectangle {
        implicitWidth: parent.width
        topLeftRadius: 12
        topRightRadius: 12
        color: "transparent"

        Rectangle {
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width / 2
            height: 4

            radius: 4

            color: Constants.tab
            visible: index === tabBar.currentIndex
        }
    }
}
