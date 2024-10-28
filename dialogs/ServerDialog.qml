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

    standardButtons: Dialog.Ok | Dialog.Cancel

    Shortcut {
        onActivated: reject()
        sequence: Qt.Key_Escape
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        RowLayout {
            Layout.fillWidth: true
            uniformCellSizes: true
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

        RowLayout {
            Layout.fillWidth: true
            uniformCellSizes: true
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

        CheckBox {
            Layout.fillWidth: true
            id: useTeam
            text: "Use Team Number?"

            checked: settings.useTeam

            font.pixelSize: 17
        }
    }
}
