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

#include "server.h"
#include "servermodel.h"
#include "serverfiltermodel.h"
#include <QJsonArray>
#include <QJsonValue>
#include <QTimeZone>
#include <QHostAddress>

#define KEY_NAME "name"
#define KEY_HOSTNAME "hostName"
#define KEY_IPV6 "ipv6"
#define KEY_IPV4 "ipv4"
#define KEY_NOTES "notes"
#define KEY_LASTCHECK "lastCheck"
#define KEY_LASTRESULT "lastResult"

Server::Server(QObject *parent) : QObject(parent)
{

}

Server::Server(const QJsonObject &obj, QObject *parent) :
    QObject(parent),
    m_name(obj.value(QStringLiteral(KEY_NAME)).toString()),
    m_hostName(obj.value(QStringLiteral(KEY_HOSTNAME)).toString()),
    m_ipv4(obj.value(QStringLiteral(KEY_IPV4)).toString()),
    m_ipv6(obj.value(QStringLiteral(KEY_IPV6)).toString()),
    m_notes(obj.value(QStringLiteral(KEY_NOTES)).toString()),
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
    m_lastCheck(QDateTime::fromSecsSinceEpoch(static_cast<qint64>(obj.value(QStringLiteral(KEY_LASTCHECK)).toDouble(0)), Qt::UTC)),
#else
    m_lastCheck(QDateTime::fromTime_t(static_cast<uint>(obj.value(QStringLiteral(KEY_LASTCHECK)).toDouble(0)), Qt::UTC)),
#endif
    m_lastResult(static_cast<Server::CheckResult>(obj.value(QStringLiteral(KEY_LASTRESULT)).toInt(-1)))
{

}

Server::Server(const QString &name, const QString &hostName, const QString &ipv4, const QString &ipv6, const QString &notes, QObject *parent) :
    QObject(parent),
    m_name(name),
    m_hostName(hostName),
    m_ipv4(ipv4),
    m_ipv6(ipv6),
    m_notes(notes)
{

}

Server::~Server()
{

}

QJsonObject Server::toJson() const
{
    return QJsonObject({
                           {QStringLiteral(KEY_NAME), m_name},
                           {QStringLiteral(KEY_HOSTNAME), m_hostName},
                           {QStringLiteral(KEY_IPV4), m_ipv4},
                           {QStringLiteral(KEY_IPV6), m_ipv6},
                           {QStringLiteral(KEY_NOTES), m_notes},
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
                           {QStringLiteral(KEY_LASTCHECK), static_cast<double>(m_lastCheck.toSecsSinceEpoch())},
#else
                           {QStringLiteral(KEY_LASTCHECK), static_cast<double>(m_lastCheck.toTime_t())},
#endif
                           {QStringLiteral(KEY_LASTRESULT), static_cast<int>(m_lastResult)}
                       });
}

bool Server::save() const
{
    ServerModel *sm = qobject_cast<ServerModel*>(parent());
    if (sm) {
        return sm->save();
    } else {
        return false;
    }
}

bool Server::update(const QString &name, const QString &hostName, const QString &ipv4, const QString &ipv6, const QString &notes)
{
    ServerModel *sm = qobject_cast<ServerModel*>(parent());

    if (sm) {
        sm->clearError();
    }

    const QString oldName = m_name;
    const QString oldHostName = m_hostName;
    const QString oldIpv4 = m_ipv4;
    const QString oldIpv6 = m_ipv6;
    const QString oldNotes = m_notes;

    const QString _name = name.trimmed();
    if (_name.isEmpty()) {
        qWarning("%s", "Server name must not be empty.");
        if (sm) {
            //: error message when updating a server
            //% "Can not update server. The name of the server must not be empty."
            sm->setError(qtTrId("bikorung-edit-server-failed-empty-name"), ServerModel::InputError);
        }
        return false;
    }

    const QString _hostName = hostName.trimmed();

    const QString _ipv4 = ipv4.trimmed();
    if (!_ipv4.isEmpty()) {
        QHostAddress a(_ipv4);
        if (a.isNull() || a.protocol() != QAbstractSocket::IPv4Protocol) {
            qWarning("%s", "Invalid IPv4 address.");
            if (sm) {
                //: error message when updating a server
                //% "Can not update server. The IPv4 address is not valid."
                sm->setError(qtTrId("bikorung-edit-server-invalid-ipv4"), ServerModel::InputError);
            }
            return false;
        }
    }

    const QString _ipv6 = ipv6.trimmed();
    if (!_ipv6.isEmpty()) {
        QHostAddress a(_ipv6);
        if (a.isNull() || a.protocol() != QAbstractSocket::IPv6Protocol) {
            qWarning("%s", "Invalid IPv6 address.");
            if (sm) {
                //: error message when updating a server
                //% "Can not update server. The IPv6 address is not valid."
                sm->setError(qtTrId("bikorung-edit-server-invalid-ipv6"), ServerModel::InputError);
            }
            return false;
        }
    }

    const QString _notes = notes.trimmed();

    setName(_name);
    setHostName(_hostName);
    setIpv4(_ipv4);
    setIpv6(_ipv6);
    setNotes(_notes);

    if (!save()) {
        setName(oldName);
        setHostName(oldHostName);
        setIpv4(oldIpv4);
        setIpv6(oldIpv6);
        setNotes(oldNotes);
        return false;
    }

    if (sm) {
        ServerFilterModel *sfm = qobject_cast<ServerFilterModel*>(sm->parent());
        if (sfm) {
            sfm->invalidate();
        }
    }

    return true;
}

QString Server::name() const
{
    return m_name;
}

void Server::setName(const QString &name)
{
    if (name != m_name) {
        m_name = name;
        emit nameChanged(this->name());
    }
}

QString Server::hostName() const
{
    return m_hostName;
}

void Server::setHostName(const QString &hostName)
{
    if (hostName != m_hostName) {
        m_hostName = hostName;
        emit hostNameChanged(this->hostName());
    }
}

QString Server::ipv4() const
{
    return m_ipv4;
}

void Server::setIpv4(const QString &ipv4)
{
    if (ipv4 != m_ipv4) {
        m_ipv4 = ipv4;
        emit ipv4Changed(this->ipv4());
    }
}

QString Server::ipv6() const
{
    return m_ipv6;
}

void Server::setIpv6(const QString &ipv6)
{
    if (ipv6 != m_ipv6) {
        m_ipv6 = ipv6;
        emit ipv4Changed(this->ipv6());
    }
}

QString Server::notes() const
{
    return m_notes;
}

void Server::setNotes(const QString &notes)
{
    if (notes != m_notes) {
        m_notes = notes;
        emit notesChanged(this->notes());
    }
}

QDateTime Server::lastCheck() const
{
    return m_lastCheck.toTimeZone(QTimeZone::systemTimeZone());
}

void Server::setLastCheck(const QDateTime &lastCheck)
{
    if (lastCheck != m_lastCheck) {
        m_lastCheck = lastCheck.toUTC();
        emit lastCheckChanged(this->lastCheck());
    }
}

Server::CheckResult Server::lastResult() const
{
    return m_lastResult;
}

void Server::setLastResult(Server::CheckResult lastResult)
{
    if (lastResult != m_lastResult) {
        m_lastResult = lastResult;
        emit lastResultChanged(this->lastResult());
    }
}

#include "moc_server.cpp"
