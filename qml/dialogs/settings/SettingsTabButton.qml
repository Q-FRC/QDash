import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import QFRCDashboard

TabButton {
    required property string label

    id: button

    contentItem: ColumnLayout {
        IconButton {
            label: button.label

            Layout.maximumHeight: 75 * Constants.scalar
            Layout.maximumWidth: 75 * Constants.scalar

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            onClicked: button.clicked()
        }

        Text {
            font.pixelSize: Math.round(18 * Constants.scalar)
            text: label

            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

            color: Constants.palette.text
        }
    }

    background: Rectangle {
        // implicitWidth: parent.width
        topLeftRadius: 12 * Constants.scalar
        topRightRadius: 12 * Constants.scalar
        color: "transparent"
    }
}
