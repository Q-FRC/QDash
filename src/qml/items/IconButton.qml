import QtQuick
import QtQuick.Controls.Material

import constants

Button {
    required property string label

    icon.source: "qrc:/" + label
    icon.width: 45 * Constants.scalar
    icon.height: 45 * Constants.scalar
    icon.color: Constants.accent

    background: Item {}
}
