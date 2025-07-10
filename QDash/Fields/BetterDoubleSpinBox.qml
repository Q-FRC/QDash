import QtQuick
import QtQuick.Controls.Material

import QDash.Items

DoubleSpinBox {
    id: dsb

    property bool connected: true
    property bool valid: true
    property string label: ""
    enabled: connected

    contentItem: BetterSpinBox {
        valid: parent.valid
        label: parent.label
        connected: parent.connected
    }
}
