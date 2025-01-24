import QtQuick
import QtQuick.Controls

import QFRCDashboard

DoubleSpinBox {
    property bool valid: true

    contentItem: BetterSpinBox {
        valid: parent.valid
    }
}
