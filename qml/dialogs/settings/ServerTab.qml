import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QFRCDashboard

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
            implicitWidth: 180 * Constants.scalar
            id: team

            from: 0
            to: 99999

            label: "Team Number"

            bindedProperty: "team"
            bindTarget: settings
        }

        LabeledTextField {
            implicitWidth: 230 * Constants.scalar
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
        implicitHeight: 50 * Constants.scalar
        implicitWidth: 250 * Constants.scalar

        label: "Connection Mode"

        bindedProperty: "mode"
        bindTarget: settings

        choices: ["IP Address", "Team Number", "Driver Station"]
    }
}
