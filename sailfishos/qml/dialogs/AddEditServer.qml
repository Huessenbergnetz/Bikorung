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
import "../parts"

Dialog {
    id: addServerDialog
    property Server server
    canAccept: serverName.text.length > 0

    onAccepted: {
        if (server) {
            server.update(serverName.text, hostName.text, ipv4.text, ipv6.text, notes.text);
        } else {
           servers.addServer(serverName.text, hostName.text, ipv4.text, ipv6.text, notes.text);
        }
    }

    SilicaFlickable {
        id: addServerFlick
        anchors.fill: parent
        contentWidth: width
        contentHeight: addServerCol.height

        VerticalScrollDecorator { flickable: addServerFlick; page: addServerDialog }

        Column {
            id: addServerCol
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
                dialog: addServerDialog
                flickable: addServerFlick
                acceptText: !server ? qsTrId("bikorung-add-server") : defaultAcceptText
            }

            TextFieldWithDesc {
                id: serverName
                text: server ? server.name : ""
                //: label for a text field when adding or editing a server
                //% "Server name"
                label: qsTrId("bikorung-server-name")
                //: description for the name text field when adding or editing a server
                //% "The server name has not to be the host name of the server. It is used as an identifier."
                description: qsTrId("bikorung-server-name-desc")
                inputMethodHints: Qt.ImhNoPredictiveText
                validator: RegExpValidator { regExp: /^\S+.*/ }
                enterIconSource: "image://theme/icon-m-enter-next"
                onEnterClicked: hostName.inputFocus = true
            }

            TextFieldWithDesc {
                id: hostName
                text: server ? server.hostName : ""
                //: label for a text field when adding or editing a server
                //% "Host name"
                label: qsTrId("bikorung-server-hostname")
                //: description for the host name text field when adding or editing a server
                //% "The host name of the server. This will be used as convenient placeholder when adding new checks."
                description: qsTrId("bikorung-server-hostname-desc")
                inputMethodHints: Qt.ImhNoPredictiveText|Qt.ImhUrlCharactersOnly
                enterIconSource: "image://theme/icon-m-enter-next"
                onEnterClicked: ipv4.inputFocus = true
            }

            TextFieldWithDesc {
                id: ipv4
                text: server ? server.ipv4 : ""
                //: label for a text field when adding or editing a server
                //% "IPv4 address"
                label: qsTrId("bikorung-server-ipv4")
                //: description for the ipv4 text field when adding or editing a server
                //% "The IPv4 address of the server. This will be used as convenient placeholder when adding new checks."
                description: qsTrId("bikorung-server-ipv4-desc")
                inputMethodHints: Qt.ImhNoPredictiveText
                enterIconSource: "image://theme/icon-m-enter-next"
                onEnterClicked: ipv6.inputFocus = true
            }

            TextFieldWithDesc {
                id: ipv6
                text: server ? server.ipv6 : ""
                //: label for a text field when adding or editing a server
                //% "IPv6 address"
                label: qsTrId("bikorung-server-ipv6")
                //: description for the ipv6 text field when adding or editing a server
                //% "The IPv6 address of the server. This will be used as convenient placeholder when adding new checks."
                description: qsTrId("bikorung-server-ipv6-desc")
                inputMethodHints: Qt.ImhNoPredictiveText
                enterIconSource: "image://theme/icon-m-enter-next"
                onEnterClicked: notes.focus = true
            }

            Item {
                width: parent.width
                height: notes.height + notesDesc.height + Theme.paddingLarge

                TextArea {
                    id: notes
                    text: server ? server.notes : ""
                    anchors { left: parent.left; right: parent.right }
                    //: label for a text area when adding or editing a server
                    //% "Notes"
                    label: qsTrId("bikorung-server-notes"); placeholderText: label
                    EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                    EnterKey.enabled: addServerDialog.canAccept
                    EnterKey.onClicked: addServerDialog.accept()
                }

                Text {
                    id: notesDesc
                    //: description for the notes text area when adding or editing a server
                    //% "Additional notes for the server."
                    text: qsTrId("bikorung-server-notes-desc")
                    anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; top: notes.bottom }
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeExtraSmall
                }
            }
        }
    }
}
