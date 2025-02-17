import QtQuick
import QtQuick.Controls.Material

import QFRCDashboard

SpinBox {

    id: spin
    property bool valid: true
    property string label: ""

    from: -0x7FFFFFFF
    to: 0x7FFFFFFF

    contentItem: BetterTextField {
        text: parent.textFromValue(parent.value, parent.locale)

        placeholderText: parent.label

        font: parent.font
        color: Constants.palette.text
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        inputMethodHints: Qt.ImhFormattedNumbersOnly

        onEditingFinished: {
            spin.value = parseFloat(text.replace(/,/g, ""))
            valueModified()
        }
    }

    up.indicator: Item {}

    down.indicator: Item {}

    background: Item {}
}
