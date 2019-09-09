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

#ifndef SERVERFILTERMODEL_H
#define SERVERFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "servermodel.h"

class ServerFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_DISABLE_COPY(ServerFilterModel)
    Q_PROPERTY(ServerModel::Error lastError READ lastError NOTIFY lastErrorChanged)
    Q_PROPERTY(QString lastErrorString READ lastErrorString NOTIFY lastErrorStringChanged)
public:
    explicit ServerFilterModel(QObject *parent = nullptr);
    ~ServerFilterModel() override;

    Q_INVOKABLE bool addServer(const QString &name, const QString &hostName, const QString &ipv4, const QString &ipv6, const QString &notes);
    Q_INVOKABLE bool deleteServer(int idx);

    Q_INVOKABLE bool save();

    Q_INVOKABLE void clearError();

    ServerModel::Error lastError() const;
    QString lastErrorString() const;

signals:
    void lastErrorChanged(ServerModel::Error error);
    void lastErrorStringChanged(const QString &errorString);

protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    ServerModel* m_sm;
};

#endif // SERVERFILTERMODEL_H
