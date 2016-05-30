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

#include "iqwamprealm_p.h"
#include "iqwamplocalclient.h"
#include "iqwampcallee.h"

IqWampRealmPrivate::IqWampRealmPrivate(IqWampRealm *parent):
    QObject(parent),
    q_ptr(parent),
    m_name(""),
    m_localClient(new IqWampLocalClient(this, parent)),
    m_subscriptions(new IqWampSubscriptions(this)),
    m_registrations(new IqWampRegistrations(this)),
    m_broker(new IqWampBroker(this)),
    m_dialer(new IqWampDialer(this))
{
}

void IqWampRealmPrivate::onClientDisconnected()
{
    IqWampCallee *callee = qobject_cast<IqWampCallee *>(sender());
    if (!callee)
        return;

    subscriptions()->remove(callee);
    registrations()->remove(callee);

    m_clients.remove(callee);
    callee->deleteLater();
}

//void IqWampRealmPrivate::onClosed()
//{
//}
//
//void IqWampRealmPrivate::onClientSubscribed(int subscriptionId)
//{
//    IqWampCallee *client = qobject_cast<IqWampCallee *>(sender());
//    if (!client)
//        return;
//    if (hasSubscription(subscriptionId, client))
//        return;
//
//    m_clientSubscription.insert(subscriptionId, client);
//}
//
//void IqWampRealmPrivate::onClientRegistered(int registrationId)
//{
//    IqWampCallee *client = qobject_cast<IqWampCallee *>(sender());
//    if (!client)
//        return;
//    if (hasRegistration(registrationId))
//        return;
//
//    m_clientRegistrations.insert(registrationId, client);
//}
//
//void IqWampRealmPrivate::onClientUnSubscribed(int subscriptionId)
//{
//    IqWampCallee *client = qobject_cast<IqWampCallee *>(sender());
//    if (!client)
//        return;
//
//    m_clientSubscription.remove(subscriptionId, client);
//}
//
//void IqWampRealmPrivate::sendEvent(int subscriptionId,
//                                      int publicationId,
//                                      const QJsonArray &arguments,
//                                      const QJsonObject &argumentsKw)
//{
//    QList<IqWampCallee *> clientsToSend = m_clientSubscription.values(subscriptionId);
//
//    for(IqWampCallee *client: clientsToSend) {
//        client->sendEvent(subscriptionId, publicationId, arguments, argumentsKw);
//    }
//}

void IqWampRealmPrivate::addClient(IqWampCallee *client)
{
    client->setParent(this);
    Q_Q(IqWampRealm);
    client->setRealm(q);
    connect(client, &IqWampCallee::disconnected, this, &IqWampRealmPrivate::onClientDisconnected);

//    connect(client, &IqWampCallee::subscribed, this, &IqWampRealmPrivate::onClientSubscribed);
//    connect(client, &IqWampCallee::unSubscribed, this, &IqWampRealmPrivate::onClientUnSubscribed);
//    connect(client, &IqWampCallee::publish, this, &IqWampRealmPrivate::sendEvent);
//
//    connect(client, &IqWampCallee::registered, this, &IqWampRealmPrivate::onClientRegistered);
}

//int IqWampRealmPrivate::subscriptionId(const QString &topic)
//{
//    if (m_subscriptionIds.contains(topic))
//        return m_subscriptionIds[topic];
//
//    m_lastSubscriptionId++;
//    m_subscriptionIds[topic] = m_lastSubscriptionId;
//    m_subscriptionTopics[m_lastSubscriptionId] = topic;
//
//    return m_lastSubscriptionId;
//}
//
//QString IqWampRealmPrivate::subscriptionTopic(int id)
//{
//    if (m_subscriptionTopics.contains(id))
//        return m_subscriptionTopics[id];
//
//    return "";
//}

//QList<IqWampCallee *> IqWampRealmPrivate::subscribers(int subscriptionId) const
//{
//    return m_clientSubscription.values(subscriptionId);
//}
//
//bool IqWampRealmPrivate::hasSubscription(int subscriptionId, const IqWampCallee *client) const
//{
//    if (!client)
//        return m_subscriptionTopics.contains(subscriptionId);
//
//    return m_clientSubscription.contains(subscriptionId, const_cast<IqWampCallee *>(client));
//}
//
//bool IqWampRealmPrivate::hasTopic(const QString &topic) const
//{
//    return m_subscriptionIds.contains(topic);
//}
//
//int IqWampRealmPrivate::newPublicationId()
//{
//    return m_lastPublicationId++;
//}
//
//int IqWampRealmPrivate::newInvocationId()
//{
//    return m_lastInvocationId++;
//}
//
//void IqWampRealmPrivate::publish(const QString &topic, const QJsonObject &argumentsKw)
//{
//    int subscription = subscriptionId(topic);
//    int publicationId = newPublicationId();
//
//    sendEvent(subscription, publicationId, QJsonArray(), argumentsKw);
//}

IqWampSubscriptions *IqWampRealmPrivate::subscriptions() const
{
    return m_subscriptions;
}

IqWampRegistrations *IqWampRealmPrivate::registrations() const
{
    return m_registrations;
}

IqWampBroker *IqWampRealmPrivate::broker() const
{
    return m_broker;
}

IqWampDialer *IqWampRealmPrivate::dialer() const
{
    return m_dialer;
}


//int IqWampRealmPrivate::createRegistrationId(const QString &procedure)
//{
//    m_lastRegistrationId++;
//    m_registrationIds[procedure] = m_lastRegistrationId;
//    m_registrationProcedures[m_lastRegistrationId] = procedure;
//
//    return m_lastRegistrationId;
//}
//
//int IqWampRealmPrivate::registrationId(const QString &procedure) const
//{
//    if (m_registrationIds.contains(procedure))
//        return m_registrationIds[procedure];
//
//    return -1;
//}
//
//QString IqWampRealmPrivate::registrationProcedure(int registrationId) const
//{
//    if (m_registrationProcedures.contains(registrationId))
//        return m_registrationProcedures[registrationId];
//
//    return "";
//}
//
//bool IqWampRealmPrivate::hasRegistration(int registrationId, const IqWampCallee *client) const
//{
//    if (!client)
//        return m_registrationProcedures.contains(registrationId);
//
//    return m_clientRegistrations[registrationId] == client;
//}
//
//bool IqWampRealmPrivate::hasRegistration(const QString &procedure) const
//{
//    return m_registrationIds.contains(procedure);
//}

