import QtQuick 6.7
import QtQuick.Controls 6.6
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QFRCDashboard

TabButton {
    required property string label

    id: button

    height: 40

    contentItem: Row {
        Image {
            source: "qrc:/" + label + (index === tabBar.currentIndex ? "Light" : "Dark")
            width: 25
            height: 25
        }

        Label {
            font.pixelSize: 18
            text: label

            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter

            color: index === tabBar.currentIndex ? Constants.tab : "white"
        }
    }

    background: Rectangle {
        implicitWidth: parent.width
        topLeftRadius: 12
        topRightRadius: 12
        color: index !== tabBar.currentIndex ? Constants.tab : "white"
    }
}
