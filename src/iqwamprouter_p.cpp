/**********************************************************************************
 **
 ** Copyright © 2016 Pavel A. Puchkov 
 **
 ** This file is part of IqMeteoServer. 
 **
 ** IqMeteoServer is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** IqMeteoServer is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with IqMeteoServer.  If not, see <http://www.gnu.org/licenses/>.
 **
 **********************************************************************************/

#include "iqwamprouter_p.h"

IqWampRouterPrivate::IqWampRouterPrivate(QObject *parent):
    QObject(parent),
    m_server(Q_NULLPTR)
{
}

bool IqWampRouterPrivate::listen(quint16 port, QWebSocketServer::SslMode secureMode)
{
    if (m_server)
        return false;

    m_server = new QWebSocketServer(QStringLiteral("IqWamp"), secureMode, this);

    if (!m_server->listen(QHostAddress::Any, port)) {
        delete m_server;
        return false;
    }

    connect(m_server, &QWebSocketServer::newConnection, this, &IqWampRouterPrivate::onNewConnection);
    connect(m_server, &QWebSocketServer::closed, this, &IqWampRouterPrivate::onClosed);

    return true;
}

void IqWampRouterPrivate::onNewConnection()
{
    while (m_server->hasPendingConnections()) {
        QWebSocket *socket = m_server->nextPendingConnection();
#ifdef IQWAMP_DEBUG_MODE
        qDebug() << "New connection from " << socket->peerAddress().toString();
#endif
        IqWampCallee *client = new IqWampCallee(socket, this);
        connect(client, &IqWampCallee::hello, this, &IqWampRouterPrivate::onClientHello);

        m_clients << client;
    }
}


void IqWampRouterPrivate::addRealm(IqWampRealm *realm)
{
    Q_CHECK_PTR(realm);
    if (realm) {
        realm->setParent(this);
        m_realms[realm->name()] = realm;
    }
}

void IqWampRouterPrivate::onClosed()
{

}

void IqWampRouterPrivate::onClientHello(const QString &realm)
{
    IqWampCallee *client = qobject_cast<IqWampCallee *>(sender());
    IqWampRealm *realmObject = m_realms[realm];
    if (realmObject) {
        realmObject->addCallee(client);
        client->sendWelcome();

        disconnect(client, 0, this, 0);
        m_clients.remove(client);
    } else {
        client->sendAbort(IqWamp::Errors::NoSuchRealm);
        client->closeConnection();
        client->deleteLater();

        m_clients.remove(client);
    }
}
