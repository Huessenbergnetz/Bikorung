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

#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDateTime>
#include <QJsonObject>

class Server : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Server)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QDateTime lastCheck READ lastCheck WRITE setLastCheck NOTIFY lastCheckChanged)
    Q_PROPERTY(Server::CheckResult lastResult READ lastResult WRITE setLastResult NOTIFY lastResultChanged)
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)
    Q_PROPERTY(QString ipv4 READ ipv4 WRITE setIpv4 NOTIFY ipv4Changed)
    Q_PROPERTY(QString ipv6 READ ipv6 WRITE setIpv6 NOTIFY ipv6Changed)
    Q_PROPERTY(QString notes READ notes WRITE setNotes NOTIFY notesChanged)
public:
    enum CheckResult {
        NeverChecked = -1,
        SuccessChecked = 0,
        FailedChecked = 1
    };
    Q_ENUM(CheckResult)

    explicit Server(QObject *parent = nullptr);
    explicit Server(const QJsonObject &obj, QObject *parent = nullptr);
    explicit Server(const QString &name, const QString &hostName, const QString &ipv4, const QString &ipv6, const QString &notes, QObject *parent = nullptr);
    ~Server() override;

    QJsonObject toJson() const;

    Q_INVOKABLE bool save() const;

    Q_INVOKABLE bool update(const QString &name, const QString &hostName, const QString &ipv4, const QString &ipv6, const QString &notes);

    QString name() const;
    QDateTime lastCheck() const;
    Server::CheckResult lastResult() const;
    QString hostName() const;
    QString ipv4() const;
    QString ipv6() const;
    QString notes() const;

public slots:
    void setName(const QString &name);
    void setLastCheck(const QDateTime &lastCheck);
    void setLastResult(Server::CheckResult lastResult);
    void setHostName(const QString &hostName);
    void setIpv4(const QString &ipv4);
    void setIpv6(const QString &ipv6);
    void setNotes(const QString &notes);

signals:
    void nameChanged(const QString &name);
    void lastCheckChanged(const QDateTime &lastCheck);
    void lastResultChanged(Server::CheckResult lastResult);
    void hostNameChanged(const QString &hostName);
    void ipv4Changed(const QString &ipv4);
    void ipv6Changed(const QString &ipv6);
    void notesChanged(const QString &notes);

private:
    QString m_name;
    QString m_hostName;
    QString m_ipv4;
    QString m_ipv6;
    QString m_notes;
    QDateTime m_lastCheck;
    Server::CheckResult m_lastResult = Server::NeverChecked;
};

#endif // SERVER_H
