import QtQuick
import QtQuick.Controls.Material
import QFRCDashboard

ComboBox {
    property bool valid: true
    property string label: ""

    id: control

    background: Item {}

    contentItem: TextField {
        leftPadding: 3
        rightPadding: 3
        topPadding: Material.textFieldVerticalPadding
        bottomPadding: Material.textFieldVerticalPadding

        placeholderText: control.label
        placeholderTextColor: enabled
                              && activeFocus ? Constants.accent : Qt.darker(
                                                   Constants.palette.text, 1.3)

        text: control.editable ? control.editText : control.displayText

        enabled: control.editable
        autoScroll: control.editable
        readOnly: control.down
        inputMethodHints: control.inputMethodHints
        validator: control.validator
        selectByMouse: control.selectTextByMouse

        color: control.enabled ? control.Material.foreground : control.Material.hintTextColor
        selectionColor: control.Material.accentColor
        selectedTextColor: control.Material.primaryHighlightedTextColor
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: "AlignHCenter"

        background: Item {}
    }

    FieldFooter {
        valid: parent.valid
    }

    delegate: BetterMenuItem {}
}
