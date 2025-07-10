import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QDash.Constants
import QDash.Config

ColumnLayout {
    spacing: 5

    function accept() {
        team.accept()
        ip.accept()
        settings.mode = mode.currentIndex
    }

    function open() {
        team.open()
        ip.open()
        mode.currentIndex = settings.mode
    }

    RowLayout {
        Layout.leftMargin: 4
        Layout.fillWidth: true

        LabeledSpinBox {
            implicitWidth: 230
            id: team

            from: 0
            to: 99999

            label: "Team Number"

            bindedProperty: "team"
            bindTarget: settings
        }

        LabeledTextField {
            implicitWidth: 230
            id: ip

            label: "IP Address"

            horizontalAlignment: "AlignHCenter"

            bindedProperty: "ip"
            bindTarget: settings

            validator: RegularExpressionValidator {
                regularExpression: /^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$/
            }
        }
    }

    LabeledComboBox {
        id: mode
        implicitHeight: 50
        implicitWidth: 250

        label: "Connection Mode"

        bindedProperty: "mode"
        bindTarget: settings

        choices: ["IP Address", "Team Number", "Driver Station"]
    }
}
