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

    Column {
        anchors.fill: parent
        spacing: 5

        Text {
            text: "Input new tab name:"
            font.pixelSize: 20
            color: Constants.palette.text
        }

        TextField {
            id: tabName
            font.pixelSize: 20
            placeholderText: "New Tab"
        }

        DialogButtonBox {
            Shortcut {
                context: Qt.WidgetWithChildrenShortcut
                sequences: [Qt.Key_Escape]

                onActivated: tabNameDialog.reject()
            }

            Button {
                text: qsTr("Ok")
                DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            }
            Button {
                text: qsTr("Cancel")
                DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
            }

            width: tabName.width
            font.pixelSize: 15

            onAccepted: tabNameDialog.accept()
            onRejected: tabNameDialog.reject()
        }
    }
}
