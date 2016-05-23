/**********************************************************************************
 **
 ** Copyright © 2016 Pavel A. Puchkov 
 **
 ** This file is part of IqWamp.
 **
 ** IqWamp is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** IqWamp is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with IqWamp.  If not, see <http://www.gnu.org/licenses/>.
 **
 **********************************************************************************/

#ifndef IQWAMPROUTER_P_H
#define IQWAMPROUTER_P_H

#include <QObject>
#include <QWebSocketServer>
#include "iqwamprealm.h"

class IqWampRouterPrivate: public QObject
{
    Q_OBJECT
public:
    explicit IqWampRouterPrivate(QObject *parent = 0);

    bool listen(quint16 port, QWebSocketServer::SslMode secureMode = QWebSocketServer::NonSecureMode);

    void addRealm(IqWampRealm *realm);

private:
    void onNewConnection();
    void onClientHello(const QString &realm);
    void onClosed();

private:
    QWebSocketServer *m_server;

    QSet<IqWampCallee *> m_clients;
    QHash<QString, IqWampRealm *> m_realms;

};

#endif //IQWAMPROUTER_P_H
