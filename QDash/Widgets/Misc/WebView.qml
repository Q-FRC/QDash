import QtCore
import QtQuick 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 6.6

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import QDash.Constants

import QtWebEngine

BaseWidget {
    id: widget

    property string item_url: ""
    connected: true

    BetterMenu {
        id: webMenu
        title: "Web Actions"

        MenuItem {
            text: "Back"
            onTriggered: web.goBack()
        }

        MenuItem {
            text: "Forward"
            onTriggered: web.goForward()
        }

        MenuItem {
            text: "Reload"
            onTriggered: web.reload()
        }

        MenuItem {
            text: "Sync URL"
            onTriggered: item_url = web.url
        }
    }

    Component.onCompleted: {
        rcMenu.addMenu(webMenu)
    }

    Item {
        anchors {
            top: titleField.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right

            leftMargin: 10
            rightMargin: 10
        }

        WebEngineView {
            id: web

            anchors.fill: parent
            url: item_url

            // TODO: profiles
            profile: WebEngineProfile {
                storageName: "QDash"
                persistentCookiesPolicy: WebEngineProfile.ForcePersistentCookies
                offTheRecord: false
            }
        }
    }

    BaseConfigDialog {
        id: config

        content: ColumnLayout {
            id: layout
            spacing: 12
            anchors.fill: parent
            anchors.leftMargin: 2
            clip: true

            SectionHeader {
                label: "Font Settings"
            }

            LabeledSpinBox {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                id: titleFontField

                label: "Title Font Size"

                bindedProperty: "item_titleFontSize"
                bindTarget: widget
            }

            SectionHeader {
                label: "Web View Settings"
            }

            LabeledTextField {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                id: urlField

                label: "URL"

                bindedProperty: "item_url"
                bindTarget: widget
            }
        }
    }
}
