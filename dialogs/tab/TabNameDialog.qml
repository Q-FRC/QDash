import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QFRCDashboard

AnimatedDialog {
    id: tabNameDialog

    title: "New Tab"

    height: 195 * Constants.scalar
    width: 250 * Constants.scalar

    property string text

    function openDialog() {
        open()
        tabName.focus = true
        tabName.text = ""
    }

    function finish() {
        text = tabName.text

        accept()
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    Shortcut {
        onActivated: reject()
        sequence: Qt.Key_Escape
    }

    Shortcut {
        onActivated: finish()
        sequence: Qt.Key_Return
    }

    LabeledTextField {
        anchors.fill: parent

        id: tabName
        font.pixelSize: Math.round(20 * Constants.scalar)

        onAccepted: tabNameDialog.finish()

        label: "Tab Name"

        bindTarget: parent
        bindedProperty: "text"
    }
}
