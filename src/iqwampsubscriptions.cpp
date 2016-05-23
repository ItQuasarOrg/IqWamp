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

#include "iqwampsubscriptions.h"

IqWampSubscriptions::IqWampSubscriptions(QObject *parent):
    QObject(parent),
    m_lastSubscriptionId(0)
{
}

bool IqWampSubscriptions::hasSubscription(const QString &topic) const
{
    return m_subscriptionOnTopic.contains(topic);
}

bool IqWampSubscriptions::hasSubscription(int id) const
{
    return m_subscriptionOnId.contains(id);
}

QSharedPointer<IqWampSubscription> IqWampSubscriptions::subscription(const QString &topic) const
{
    return m_subscriptionOnTopic[topic];
}

QSharedPointer<IqWampSubscription> IqWampSubscriptions::subscription(int id) const
{
    return m_subscriptionOnId[id];
}

QSharedPointer<IqWampSubscription> IqWampSubscriptions::create(const QString &topic, IqWampCallee *callee)
{
    int id = m_lastSubscriptionId++;
    QSharedPointer<IqWampSubscription> subscription (new IqWampSubscription(id, topic, callee));
    m_subscriptionOnTopic[topic] = subscription;
    m_subscriptionOnId[id] = subscription;
    return subscription;
}

void IqWampSubscriptions::remove(const IqWampCallee *callee)
{
    for(QSharedPointer<IqWampSubscription> &subscription: m_subscriptionOnId.values()) {
       subscription->removeCallee(callee);
    }
}

