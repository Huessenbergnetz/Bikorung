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

#include "serverfiltermodel.h"
#include "server.h"

ServerFilterModel::ServerFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    m_sm = new ServerModel(this);
    connect(m_sm, &ServerModel::lastErrorChanged, this, &ServerFilterModel::lastErrorChanged);
    connect(m_sm, &ServerModel::lastErrorStringChanged, this, &ServerFilterModel::lastErrorStringChanged);
    setSourceModel(m_sm);
    setSortRole(ServerModel::Item);
    setFilterRole(ServerModel::Item);
    setSortLocaleAware(true);
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    sort(0, Qt::AscendingOrder);
}

ServerFilterModel::~ServerFilterModel()
{

}

bool ServerFilterModel::addServer(const QString &name, const QString &hostName, const QString &ipv4, const QString &ipv6, const QString &notes)
{
    return m_sm->addServer(name, hostName, ipv4, ipv6, notes);
}

bool ServerFilterModel::deleteServer(int idx)
{
    return m_sm->deleteServer(mapToSource(index(idx, 0)).row());
}

bool ServerFilterModel::save()
{
    return m_sm->save();
}

void ServerFilterModel::clearError()
{
    m_sm->clearError();
}

ServerModel::Error ServerFilterModel::lastError() const
{
    return m_sm->lastError();
}

QString ServerFilterModel::lastErrorString() const
{
    return m_sm->lastErrorString();
}

bool ServerFilterModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    Server* l;
    Server* r;

    if (sortOrder() == Qt::AscendingOrder) {
        l = sourceModel()->data(source_left, ServerModel::Item).value<Server*>();
        r = sourceModel()->data(source_right, ServerModel::Item).value<Server*>();
    } else {
        r = sourceModel()->data(source_left, ServerModel::Item).value<Server*>();
        l = sourceModel()->data(source_right, ServerModel::Item).value<Server*>();
    }

    if (sortRole() == ServerModel::LastCheck) {
        if (l->lastCheck() < r->lastCheck()) {
            return true;
        } else if (l->lastCheck() > r->lastCheck()) {
            return false;
        }
    }

    return QString::localeAwareCompare(l->name(), r->name()) < 0;
}

#include "moc_serverfiltermodel.cpp"
