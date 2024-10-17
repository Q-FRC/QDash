import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 2.15

import QFRCDashboard

BaseWidget {
    property string item_topic

    property color item_falseColor: "#FF0000"
    property color item_trueColor: "#00FF00"

    // @disable-check M311
    property var item_shape: "Rectangle"

    property list<string> shapeChoices: ["Rectangle", "Circle", "Triangle"]

    property bool itemValue

    Menu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Checkbox"
            onTriggered: {
                model.type = "bool"
            }
        }
    }

    function setColor() {
        shape.itemColor = itemValue ? item_trueColor : item_falseColor
        shape.itemShape = item_shape
        shape.setColor()
    }

    function updateTopic(ntTopic, ntValue) {
        if (ntTopic === item_topic) {
            itemValue = ntValue
            setColor()
        }
    }

    Component.onCompleted: {
        topicStore.topicUpdate.connect(updateTopic)
        item_topic = model.topic

        rcMenu.addMenu(switchMenu)
    }

    Component.onDestruction: {
        if (topicStore !== null) {
            topicStore.topicUpdate.disconnect(updateTopic)
            topicStore.unsubscribe(item_topic)
        }
    }

    onItem_falseColorChanged: setColor()
    onItem_trueColorChanged: setColor()
    onItem_shapeChanged: setColor()

    ShapeHandler {
        id: shape

        itemShape: item_shape

        anchors {
            top: titleField.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom

            margins: 10
        }
    }

    onItem_topicChanged: {
        topicStore.unsubscribe(topic)
        topicStore.subscribe(item_topic)
        model.topic = item_topic
        updateTopic(model.topic, topicStore.getValue(model.topic))
    }
}
