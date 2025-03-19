import QtQuick
import QtQuick.Controls.Material

import QFRCDashboard

DoubleSpinBox {
    id: dsb

    property bool valid: true
    property string label: ""

    contentItem: BetterSpinBox {
        valid: parent.valid
        label: parent.label

        // onValueModified: {
        //     // dsb.value = value
        //     dsb.valueModified()
        // }
    }
}
