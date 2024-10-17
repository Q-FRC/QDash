import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 2.15

import QFRCDashboard

BaseWidget {
    property string item_topic

    // @disable-check M311
    property var item_shape: "Rectangle"

    property list<string> shapeChoices: ["Rectangle", "Circle", "Triangle"]

    property string itemValue

    property list<var> item_colorMap: []

    property string colorMapValueType: "color"
    property string colorMapValueName: "Color"

    Menu {
        id: switchMenu
        title: "Switch Widget..."

        MenuItem {
            text: "Text"
            onTriggered: {
                model.type = "string"
            }
        }
    }

    function colorFromMap(value) {
        for (let i = 0 ; i < item_colorMap.length; ++i) {
            let obj = item_colorMap[i];

            if (obj["Value"] === value) {
                return obj[colorMapValueName]
            }
        }

        return "#000000"
    }

    function setColor() {
        shape.itemColor = colorFromMap(itemValue)
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

    onItem_colorMapChanged: setColor()
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
