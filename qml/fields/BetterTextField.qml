import QtQuick
import QtQuick.Controls.Material
import QFRCDashboard

TextField {
    property bool valid: true

    placeholderTextColor: enabled && activeFocus ? Constants.accent : Qt.darker(
                                                       Constants.palette.text,
                                                       1.3)

    color: Constants.palette.text

    background: Rectangle {
        color: "transparent"
    }

    FieldFooter {
        valid: parent.valid
    }

    horizontalAlignment: "AlignHCenter"
}
