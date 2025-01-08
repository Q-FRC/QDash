import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import QFRCDashboard

TabButton {
    required property string label

    id: button

    height: 100 * Constants.scalar

    contentItem: ColumnLayout {
        Button {
            Layout.maximumHeight: 75 * Constants.scalar
            Layout.maximumWidth: 75 * Constants.scalar

            id: image
            icon.source: "qrc:/" + label
            icon.width: 45 * Constants.scalar
            icon.height: 45 * Constants.scalar
            icon.color: Constants.accent

            background: Item {}

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            onClicked: button.click()
        }

        Label {
            font.pixelSize: Math.round(18 * Constants.scalar)
            text: label

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            color: Constants.palette.text
        }
    }

    background: Rectangle {
        implicitWidth: parent.width
        topLeftRadius: 12 * Constants.scalar
        topRightRadius: 12 * Constants.scalar
        color: "transparent"

        Rectangle {
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width / 2
            height: 4 * Constants.scalar

            radius: 4 * Constants.scalar

            color: Constants.tab
            visible: index === tabBar.currentIndex
        }
    }
}
