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

ListModel {
    id: contModel
    ListElement {
        name: "Matthias Fehring (Buschmann)"
        role: ""
        section: ""
        website: "https://www.buschmann23.de"
        twitter: "buschmann23"
        github: "buschmann23"
    }

    Component.onCompleted: {
        //% "Main developer, Bikorung creator"
        contModel.get(0).role = qsTrId("intfuorit-author-role")
        //% "Author"
        contModel.get(0).section = qsTrId("Intfuorit-author-section")
    }
}

