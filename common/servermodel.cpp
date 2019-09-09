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

#include "servermodel.h"
#include "server.h"

#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QHostAddress>

ServerModel::ServerModel(QObject *parent) : QAbstractListModel(parent)
{
    init();
}

ServerModel::~ServerModel()
{
    save(m_servers);
}

int ServerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_servers.size();
}

QModelIndex ServerModel::index(int row, int column, const QModelIndex &parent) const
{
    QModelIndex idx;

    if (hasIndex(row, column, parent)) {
        idx = createIndex(row, column);
    }

    return idx;
}

QHash<int, QByteArray> ServerModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(Item, QByteArrayLiteral("item"));
    roles.insert(Name, QByteArrayLiteral("name"));
    roles.insert(HostName, QByteArrayLiteral("hostName"));
    roles.insert(IPv4, QByteArrayLiteral("ipv4"));
    roles.insert(IPv6, QByteArrayLiteral("ipv6"));
    roles.insert(Notes, QByteArrayLiteral("notes"));
    roles.insert(LastCheck, QByteArrayLiteral("lastCheck"));
    roles.insert(LastResult, QByteArrayLiteral("lastResult"));
    return roles;
}

QVariant ServerModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)

    if (!index.isValid() || (static_cast<std::vector<Server*>::size_type>(index.row()) > (m_servers.size() - 1))) {
        return QVariant();
    }

    Server *s = m_servers.at(index.row());

    switch (role) {
    case Item:
        return QVariant::fromValue<Server*>(s);
    case Name:
        return QVariant::fromValue(s->name());
    case HostName:
        return QVariant::fromValue(s->hostName());
    case IPv4:
        return QVariant::fromValue(s->ipv4());
    case IPv6:
        return QVariant::fromValue(s->ipv6());
    case Notes:
        return QVariant::fromValue(s->notes());
    case LastCheck:
        return QVariant::fromValue(s->lastCheck());
    case LastResult:
        return QVariant::fromValue<Server::CheckResult>(s->lastResult());
    default:
        return QVariant();
    }
}

bool ServerModel::addServer(const QString &name, const QString &hostName, const QString &ipv4, const QString &ipv6, const QString &notes)
{
    clearError();

    const QString _name = name.trimmed();
    if (Q_UNLIKELY(_name.isEmpty())) {
        qWarning("%s", "Server name must not be empty.");
        //: error message when adding a server
        //% "Can not add server. The name of the server must not be empty."
        setError(qtTrId("bikorung-add-server-failed-empty-name"), InputError);
        return false;
    }

    const QString _hostName = hostName.trimmed();

    const QString _ipv4 = ipv4.trimmed();
    if (!_ipv4.isEmpty()) {
        QHostAddress a(_ipv4);
        if (a.isNull() || a.protocol() != QAbstractSocket::IPv4Protocol) {
            qWarning("%s", "Invalid IPv4 address.");
            //: error message when adding a server
            //% "Can not add server. The IPv4 address is not valid."
            setError(qtTrId("bikorung-add-server-invalid-ipv4"), InputError);
            return false;
        }
    }

    const QString _ipv6 = ipv6.trimmed();
    if (!_ipv6.isEmpty()) {
        QHostAddress a(_ipv6);
        if (a.isNull() || a.protocol() != QAbstractSocket::IPv6Protocol) {
            qWarning("%s", "Can not add invalid IPv6 address.");
            //: error message when adding a server
            //% "Can not add server. The IPv6 address is not valid."
            setError(qtTrId("bikorung-add-server-invalid-ipv6"), InputError);
            return false;
        }
    }

    const QString _notes = notes.trimmed();

    qDebug("Adding new server \"%s\".", qUtf8Printable(_name));

    std::vector<Server*> servers = m_servers;

    servers.push_back(new Server(_name, _hostName, _ipv4, _ipv6, _notes, this));

    if (Q_UNLIKELY(!save(servers))) {
        qWarning("%s", "Failed to add new server.");
        return false;
    }

    emit beginInsertRows(QModelIndex(), rowCount(), rowCount());

    m_servers = servers;

    emit endInsertRows();

    return true;
}

bool ServerModel::deleteServer(int idx)
{
    clearError();

    if (idx > -1) {

        Server* s = m_servers.at(idx);

        qDebug("Removing server \"%s\".", qUtf8Printable(s->name()));

        std::vector<Server*> servers = m_servers;

        servers.erase(servers.begin() + idx);

        if (Q_UNLIKELY(!save(servers))) {
            qWarning("Failed to remove server \"%s\".", qUtf8Printable(s->name()));
            return false;
        }

        emit beginRemoveRows(QModelIndex(), idx, idx);

        m_servers = servers;

        emit endRemoveRows();

        delete s;

    } else {
        qWarning("Invalid server index %i.", idx);
        return false;
    }

    return true;
}

void ServerModel::init()
{
    clearError();
    clear();

    QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

    if (Q_UNLIKELY(!dataDir.exists())) {
        if (Q_UNLIKELY(!dataDir.mkpath(dataDir.absolutePath()))) {
            qCritical("Failed to create data directory %s", qUtf8Printable(dataDir.absolutePath()));
            //: error message when creating data directory failed
            //% "Failed to create data directory."
            setError(qtTrId("bikorung-err-failed-create-data-dir"), FilesystemError);
            return;
        }
    }

    m_dataFile.setFileName(dataDir.absoluteFilePath(QStringLiteral("servers.json")));

    if (Q_UNLIKELY(!m_dataFile.open(QIODevice::ReadOnly|QIODevice::Text))) {
        qCritical("Failed to open data file %s for reading.", qUtf8Printable(m_dataFile.fileName()));
        //: error message when opening data file failed
        //% "Failed to open data file."
        setError(qtTrId("bikorung-err-failed-open-data-file"), FilesystemError);
        return;
    }

    qDebug("Reading data from %s", qUtf8Printable(m_dataFile.fileName()));

    QByteArray jsonData = m_dataFile.readAll();

    if (Q_LIKELY(!jsonData.isEmpty())) {

        QJsonParseError jsonError;
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &jsonError);

        if (Q_LIKELY(jsonError.error == QJsonParseError::NoError)) {

            if (Q_LIKELY(jsonDoc.isArray())) {

                const QJsonArray jsonArray = jsonDoc.array();
                if (!jsonArray.empty()) {
                    emit beginInsertRows(QModelIndex(), 0, jsonArray.size() - 1);

                    m_servers.reserve(jsonArray.size());

                    for (const QJsonValue &val : jsonArray) {
                        m_servers.emplace_back(new Server(val.toObject(), this));
                    }

                    emit endInsertRows();
                }

            } else {
                qCritical("JSON data file \"%s\" does not contain an array.", qUtf8Printable(m_dataFile.fileName()));
                //: error message if json data is not an array
                //% "JSON data does not contain an array."
                setError(qtTrId("bikorung-err-json-not-array"), JsonError);
                m_dataFile.close();
                return;
            }

        } else {
            qCritical("Failed to read JSON data from data file \"%s\", offset %i: %s", qUtf8Printable(m_dataFile.fileName()), jsonError.offset, qUtf8Printable(jsonError.errorString()));
            //: error message when reading json data failed
            //% "Failed to read the JSON data because of the following error: %1"
            setError(qtTrId("bikorung-err-failed-read-json").arg(jsonError.errorString()), JsonError);
            m_dataFile.close();
            return;
        }

    } else {
        qWarning("Data file at %s is empty.", qUtf8Printable(m_dataFile.fileName()));
    }

    m_dataFile.close();
}

void ServerModel::clear()
{
    if (!m_servers.empty()) {
        emit beginRemoveRows(QModelIndex(), 0, m_servers.size() - 1);
        qDeleteAll(m_servers);
        m_servers.clear();
        emit endRemoveRows();
    }
}

bool ServerModel::save()
{
    return save(m_servers);
}

bool ServerModel::save(const std::vector<Server *> servers)
{
    qDebug("Saving server data to %s", qUtf8Printable(m_dataFile.fileName()));

    if (Q_UNLIKELY(!m_dataFile.open(QIODevice::WriteOnly|QIODevice::Text))) {
        qCritical("Failed to open data file %s for writing.", qUtf8Printable(m_dataFile.fileName()));
        setError(qtTrId("bikorung-err-failed-open-data-file"), FilesystemError);
        return false;
    }

    QJsonArray serversArray;
    if (!servers.empty()) {
        for (const Server* const server : servers) {
            serversArray.push_back(server->toJson());
        }
    }

    const QJsonDocument json(serversArray);

    qDebug("%s", json.toJson(QJsonDocument::Indented).constData());

    if (Q_UNLIKELY(m_dataFile.write(json.toJson(QJsonDocument::Indented)) < 0)) {
        qCritical("Failed to save JSON data to file \"%s\": %s", qUtf8Printable(m_dataFile.fileName()), qUtf8Printable(m_dataFile.errorString()));
        //: error message when writing json data failed
        //% "Failed to write the JSON data because of the following error: %1"
        setError(qtTrId("bikorung-err-failed-write-json").arg(m_dataFile.errorString()), FilesystemError);
        m_dataFile.close();
        return false;
    }

    if (Q_UNLIKELY(!m_dataFile.flush())) {
        qCritical("Failed to save JSON data to file \"%s\": %s", qUtf8Printable(m_dataFile.fileName()), qUtf8Printable(m_dataFile.errorString()));
        setError(qtTrId("bikorung-err-failed-write-json").arg(m_dataFile.errorString()), FilesystemError);
        m_dataFile.close();
        return false;
    }

    m_dataFile.close();

    return true;
}

ServerModel::Error ServerModel::lastError() const
{
    return m_lastError;
}

QString ServerModel::lastErrorString() const
{
    return m_lastErrorString;
}

void ServerModel::setError(const QString &errorString, ServerModel::Error error)
{
    if (errorString != m_lastErrorString) {
        m_lastErrorString = errorString;
        qDebug("Changed lastErrorString to \"%s\"", qUtf8Printable(m_lastErrorString));
        emit lastErrorStringChanged(lastErrorString());
    }

    if (error != m_lastError) {
        m_lastError = error;
        qDebug("Changed lastError to %i.", error);
        emit lastErrorChanged(lastError());
    }
}

void ServerModel::clearError()
{
    setError(QString(), NoError);
}

#include "moc_servermodel.cpp"
