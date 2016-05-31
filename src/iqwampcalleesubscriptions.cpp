/**********************************************************************************
 **
 ** Copyright © 2016 Pavel A. Puchkov 
 **
 ** This file is part of IqMeteoGui. 
 **
 ** IqMeteoGui is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** IqMeteoGui is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with IqMeteoGui.  If not, see <http://www.gnu.org/licenses/>.
 **
 **********************************************************************************/

#include "iqwampcalleesubscriptions.h"

IqWampCalleeSubscriptions::IqWampCalleeSubscriptions(QObject *parent) :
    IqWampSubscriptions<IqWampCalleeSubscription>(parent),
    m_lastSubscriptionId(0)
{
}

void IqWampCalleeSubscriptions::remove(const IqWampAbstractCallee *callee)
{
    QList<QSharedPointer<IqWampCalleeSubscription> > subscriptions = m_subscriptionOnCallee.values(callee);
    for(QSharedPointer<IqWampCalleeSubscription> &subscription: subscriptions) {
        subscription->removeCallee(callee);
    }

    m_subscriptionOnCallee.remove(callee);
}

QSharedPointer<IqWampCalleeSubscription> IqWampCalleeSubscriptions::create(const QString &topic,
                                                                           IqWampAbstractCallee *callee)
{
    int id = m_lastSubscriptionId++;
    QSharedPointer<IqWampCalleeSubscription> subscription (new IqWampCalleeSubscription(id, topic, callee));
    m_subscriptionOnCallee.insert(callee, subscription);
    addSubscription(qSharedPointerCast<IqWampSubscription>(subscription));
    return subscription;
}


