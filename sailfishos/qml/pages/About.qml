/* Intfuorit - Qt based client for haveibeenpwned.com
 * Copyright (C) 2017-2019 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/Intfuorit
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
import de.huessenbergnetz.hbnsc 1.0
import harbour.intfuorit 1.0
import "../models"

AboutPage {
    pageTitle: qsTrId("bikorung-about")
    appTitle: "Bikorung"
    //: Description on the About page
    //% "Checks status of remote services"
    appDescription: qsTrId("bikorung-app-description")
    appHomepage: "https://github.com/Huessenbergnetz/Bikorung"
    appCopyrightYearFrom: "2019"
    appCopyrightYearTo: ""
    appCopyrightHolder: "Matthias Fehring"
    appLicense: "GNU General Public License, Version 3"
    appLicenseFile: "GPLv3.qml"

    bugTrackerBaseUrl: "https://github.com/Huessenbergnetz/Bikorung/issues/"

    contactCompany: "Hüssenbergnetz"
    contactName: "Matthias Fehring"
    contactStreet: "Zum Südholz"
    contactHouseNo: "8"
    contactZIP: "34439"
    contactCity: "Willebadessen-Eissen"
    //% "Germany"
    contactCountry: qsTrId("bikorung-germany")
    contactEmail: Qt.atob("a29udGFrdEBodWVzc2VuYmVyZ25ldHouZGU=")
    contactWebsite: "www.huessenbergnetz.de"
    contactWebsiteLink: "https://www.huessenbergnetz.de/?pk_campaign=Sailfish-App-Bikorung&pk_kwd=AboutPage"

    bugUrl: "https://github.com/Huessenbergnetz/Bikorung/issues"
    translateUrl: "https://www.transifex.com/huessenbergnetz/bikorung/"

    licensesModel: LicensesModel {}
    changelogModel: ChangelogModel {}
    contributorsModel: ContributorsModel {}
    contributorsAvatarBasePath: "/usr/share/harbour-bikorung/images/contributors"

//    privacyPolicyQmlFile: Qt.resolvedUrl("PrivacyPolicy.qml")

//    paypalOrganization: "Huessenbergnetz"
//    paypalItem: "Bikorung"
//    paypalEmail: Qt.atob("a29udGFrdEBodWVzc2VuYmVyZ25ldHouZGU=")
}
