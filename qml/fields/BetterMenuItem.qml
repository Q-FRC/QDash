import QtQuick
import QtQuick.Controls.Material
import QFRCDashboard

MenuItem {
    required property var model
    required property int index
    
    width: ListView.view.width
    text: model[control.textRole]
    Material.foreground: control.currentIndex === index ? ListView.view.contentItem.Material.accent : ListView.view.contentItem.Material.foreground
    highlighted: control.highlightedIndex === index
    hoverEnabled: control.hoverEnabled
}
