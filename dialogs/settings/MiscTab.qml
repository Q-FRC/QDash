import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.6
import QtQuick.Dialogs

import QFRCDashboard

ColumnLayout {
    spacing: 5

    function accept() {
        load.accept()
    }

    function open() {
        load.open()
    }

    LabeledCheckbox {
        id: load
        label: "Load Recent Files?"

        bindTarget: settings
        bindedProperty: "loadRecent"
    }
}
