import QtQuick
import QtQuick.Controls.Material

import QDash.Constants

Button {
    required property string label

    icon.source: "qrc:/" + label
    icon.width: 45
    icon.height: 45
    icon.color: Constants.accent

    background: Item {}
}
