/* Bikorung - Qt based server service checker
 * Copyright (C) 2019 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Bikorung
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.bikorung 1.0

Page {
    id: mainPage

    allowedOrientations: Orientation.All

    SilicaListView {
        id: mainPageView
        anchors { fill: parent; bottomMargin: errorPanel.visibleSize }
        clip: errorPanel.expanded

        PullDownMenu {
            flickable: mainPageView
            MenuItem {
                //: Pull down menu entry and page header
                //% "About"
                text: qsTrId("bikorung-about")
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }
            MenuItem {
                //: Pull down menu entry and page header
                //% "Settings"
                text: qsTrId("bikorung-settings")
                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }
            MenuItem {
                //: Pull down menu enty and page hader
                //% "Add server"
                text: qsTrId("bikorung-add-server")
                onClicked: pageStack.push(Qt.resolvedUrl("../dialogs/AddEditServer.qml"))
            }
        }

        VerticalScrollDecorator { flickable: mainPageView; page: mainPage }

        header: PageHeader {
            page: mainPage
            title: "Bikorung"
        }

        model: servers

        delegate: ListItem {
            id: serverItem
            contentHeight: Theme.itemSizeMedium
            ListView.onRemove: animateRemoval(serverItem)
            menu: serverItemMenu

            function remove() {
                //: title for a remorse action to delete a server
                //% "Deleting"
                remorseAction(qsTrId("bikorung-remorse-delete-server"), function() { mainPageView.model.deleteServer(index) })
            }

            function edit() {
                pageStack.push(Qt.resolvedUrl("../dialogs/AddEditServer.qml"), {server: model.item})
            }

            Label {
                id: serverItemName
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }
                text: model.item.name
                color: serverItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                truncationMode: TruncationMode.Fade
            }

            Text {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; top: serverItemName.bottom }
                //% "Never checked"
                text: qsTrId("bikorung-server-never-checked")
                color: serverItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                font.pixelSize: Theme.fontSizeSmall
                visible: model.item.lastResult === Server.NeverChecked
            }

            Text {
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; top: serverItemName.bottom }
                text: model.item.lastCheck.toLocaleString(Qt.locale(), Locale.ShortFormat)
                color: serverItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                font.pixelSize: Theme.fontSizeSmall
                visible: model.item.lastResult !== Server.NeverChecked
            }

            Component {
                id: serverItemMenu
                ContextMenu {
                    MenuItem {
                        //: menu title to edit a server
                        //% "Edit"
                        text: qsTrId("bikorung-menu-edit-server")
                        onClicked: edit()
                    }
                    MenuItem {
                        //: menu title to delete a server
                        //% "Delete"
                        text: qsTrId("bikorung-menu-delete-server")
                        onClicked: remove()
                    }
                }
            }
        }
    }

    DockedPanel {
        id: errorPanel
        modal: true
        width: parent.width
        height: Theme.itemSizeExtraLarge + Theme.paddingLarge

        Text {
            id: errorText
            text: servers.lastErrorString
            anchors { left: parent.left; right: parent.right; top: parent.top; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; topMargin: Theme.paddingLarge }
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
        }
    }

    Connections {
        target: servers
        onLastErrorChanged: {
            if (servers.lastError === ServerModel.NoError) {
                errorPanel.hide();
            } else {
                errorPanel.show();
            }
        }
    }
}
