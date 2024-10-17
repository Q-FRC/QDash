import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QFRCDashboard

Dialog {
    id: serverDialog

    anchors.centerIn: parent

    onAccepted: {
        settings.port = port.value
        settings.useTeam = useTeam.checked
        settings.ip = ip.text
    }

    Column {
        anchors.fill: parent
        spacing: 5

        CheckBox {
            id: useTeam
            text: "Use Team Number?"

            checked: settings.useTeam

            font.pixelSize: 17
        }

        Row {
            Text {
                text: "Port:"
                font.pixelSize: 17
                color: Constants.palette.text
            }

            SpinBox {
                id: port

                value: settings.port

                from: 1
                to: 65535

                font.pixelSize: 17
            }
        }

        Row {
            Text {
                text: useTeam.checked ? "Team Number:" : "IP Address:"
                font.pixelSize: 17
                color: Constants.palette.text
            }

            TextField {
                id: ip

                text: settings.ip

                validator: RegularExpressionValidator {
                    regularExpression: useTeam.checked ? /[0-9]{1,5}/ : /^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$/
                }

                font.pixelSize: 17
            }
        }

        DialogButtonBox {
            Shortcut {
                context: Qt.WidgetWithChildrenShortcut
                sequences: [Qt.Key_Escape]

                onActivated: serverDialog.reject()
            }

            Button {
                text: qsTr("Ok")
                DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            }
            Button {
                text: qsTr("Cancel")
                DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
            }

            width: parent.width
            font.pixelSize: 15

            onAccepted: serverDialog.accept()
            onRejected: serverDialog.reject()
        }
    }
}
