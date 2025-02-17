import QtQuick
import QtQuick.Controls.Material

import QFRCDashboard

DoubleSpinBox {
    property bool valid: true
    property string label: ""

    contentItem: BetterSpinBox {
        valid: parent.valid
        label: parent.label
    }
}
