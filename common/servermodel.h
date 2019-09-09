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

#ifndef SERVERMODEL_H
#define SERVERMODEL_H

#include <QAbstractListModel>
#include <QFile>
#include <vector>

class Server;

class ServerModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(ServerModel)
    Q_PROPERTY(ServerModel::Error lastError READ lastError NOTIFY lastErrorChanged)
    Q_PROPERTY(QString lastErrorString READ lastErrorString NOTIFY lastErrorStringChanged)
public:
    explicit ServerModel(QObject *parent = nullptr);
    ~ServerModel() override;

    enum Error {
        NoError = 0,
        FilesystemError = 1,
        JsonError =  2,
        InputError = 3
    };
    Q_ENUM(Error)

    enum Roles {
        Item = Qt::UserRole + 1,
        Name,
        HostName,
        IPv4,
        IPv6,
        Notes,
        LastCheck,
        LastResult
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::UserRole) const override;

    Q_INVOKABLE bool addServer(const QString &name, const QString &hostName, const QString &ipv4, const QString &ipv6, const QString &notes);
    Q_INVOKABLE bool deleteServer(int idx);

    Q_INVOKABLE bool save();

    void setError(const QString &errorString, ServerModel::Error error);
    Q_INVOKABLE void clearError();

    ServerModel::Error lastError() const;
    QString lastErrorString() const;

signals:
    void lastErrorChanged(ServerModel::Error error);
    void lastErrorStringChanged(const QString &errorString);

private:
    void init();
    void clear();
    bool save(const std::vector<Server*> servers);

    Error m_lastError = NoError;
    QFile m_dataFile;
    QString m_lastErrorString;
    std::vector<Server*> m_servers;
};

#endif // SERVERMODEL_H
