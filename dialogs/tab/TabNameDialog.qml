import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QFRCDashboard

Dialog {
    id: tabNameDialog

    anchors.centerIn: parent

    property alias tabName: tabName

    function openUp(txt) {
        open()
        tabName.focus = true
        tabName.text = txt
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    Shortcut {
        onActivated: reject()
        sequence: Qt.Key_Escape
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        Text {
            Layout.fillWidth: true

            text: "Input new tab name:"
            font.pixelSize: 20
            color: Constants.palette.text
        }

        TextField {
            Layout.fillWidth: true

            id: tabName
            font.pixelSize: 20
            placeholderText: "New Tab"
        }
    }
}
