import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import QFRCDashboard

Rectangle {
    signal minimize
    signal maximize
    signal close
    signal beginDrag

    color: Constants.accent
    implicitHeight: 30 * Constants.scalar

    MouseArea {
        anchors.fill: parent
        onPressed: beginDrag()
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 10 * Constants.scalar
        anchors.rightMargin: 25 * Constants.scalar

        spacing: 10 * Constants.scalar

        Image {
            source: "qrc:/QFRCDashboard"
            sourceSize.height: 24 * Constants.scalar
            sourceSize.width: 24 * Constants.scalar

            fillMode: Image.PreserveAspectFit
        }

        Text {
            color: Constants.palette.text
            text: "QFRCDashboard"
            font.pixelSize: 18 * Constants.scalar

            Layout.fillWidth: true

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        Button {
            icon.source: "qrc:/Minimize"
            icon.width: 24 * Constants.scalar
            icon.height: 24 * Constants.scalar
            icon.color: Constants.palette.text

            onClicked: minimize()
        }

        Button {
            property bool maximized: false

            icon.source: maximized ? "qrc:/Unmaximize" : "qrc:/Window"
            icon.width: 24 * Constants.scalar
            icon.height: 24 * Constants.scalar
            icon.color: Constants.palette.text

            onClicked: {
                maximized = !maximized

                maximize()
            }
        }

        Button {
            icon.source: "qrc:/Close"
            icon.width: 24 * Constants.scalar
            icon.height: 24 * Constants.scalar
            icon.color: Constants.palette.text

            onClicked: close()
        }
    }
}
