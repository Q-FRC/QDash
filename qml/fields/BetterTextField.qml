import QtQuick
import QtQuick.Controls.Material
import QDash

TextField {
    property bool valid: true
    property bool connected: true

    placeholderTextColor: enabled && activeFocus ? Constants.accent : Qt.darker(
                                                       Constants.palette.text,
                                                       1.3)

    color: connected ? Constants.palette.text : Constants.palette.disabledText
    enabled: connected

    background: Rectangle {
        color: "transparent"
    }

    Behavior on color {
        ColorAnimation {
            duration: 250
        }
    }

    FieldFooter {
        valid: parent.valid
    }

    horizontalAlignment: "AlignHCenter"
}
