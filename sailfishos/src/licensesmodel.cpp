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

#include "licensesmodel.h"

LicensesModel::LicensesModel(QObject *parent) : Hbnsc::LicenseModel(parent)
{
    add(QStringLiteral("Bikorung Translations"),
        QStringLiteral("Bikorung Translators"),
        QString(),
        QUrl(QStringLiteral("https://www.transifex.com/huessenbergnetz/bikorung")),
        //: description for Bikorung Translations in the list of used 3rd party components
        //% "The translations for Bikorung are provided by the community. To see who is responsible for which translation, open the contributors page."
        qtTrId("bikorung-trans-desc"),
        QStringLiteral("Creative Commons Attribution 4.0 International Public License"),
        QStringLiteral("CC-BY-4_0.qml"),
        QUrl(QStringLiteral("https://github.com/Huessenbergnetz/Bikorung/blob/master/LICENSE.translations")),
        QUrl());

    sortLicenses();
}

LicensesModel::~LicensesModel()
{

}

#include "moc_licensesmodel.cpp"
