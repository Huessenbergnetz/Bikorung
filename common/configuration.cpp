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

#include "configuration.h"

#define KEY_LANGUAGE "display/language"

Configuration::Configuration(QObject *parent) :
    QSettings(parent),
    m_language(value(QStringLiteral(KEY_LANGUAGE)).toString())
{

}

Configuration::~Configuration()
{

}

QString Configuration::language() const
{
    return m_language;
}

void Configuration::setLanguage(const QString &language)
{
    if (language != m_language) {
        m_language = language;
        qDebug("Language changed to \"%s\".", qUtf8Printable(m_language));
        setValue(QStringLiteral(KEY_LANGUAGE), m_language);
        emit languageChanged(this->language());
    }
}

#include "moc_configuration.cpp"
