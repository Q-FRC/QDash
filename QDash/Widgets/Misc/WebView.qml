// SPDX-FileCopyrightText: Copyright 2026 crueter
// SPDX-License-Identifier: GPL-3.0-or-later
import QtCore
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.4

import QDash.Fields
import QDash.Items
import QDash.Config
import QDash.Widgets.Base
import Carboxyl.Clover

import QtWebEngine

BaseWidget {
    id: widget

    property string item_url: ""
    connected: true

    Menu {
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
    Loader {
        id: configLoader
        active: false
        asynchronous: true

        onLoaded: item.open()

        sourceComponent: BaseConfigDialog {
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
}
