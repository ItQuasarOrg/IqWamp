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

#include "iqwampsubscriptionsbase.h"

IqWampSubscriptionsBase::IqWampSubscriptionsBase(QObject *parent):
    QObject(parent)
{
}

QSharedPointer<IqWampSubscription> IqWampSubscriptionsBase::subscription(const QString &topic) const
{
    return m_subscriptionOnTopic[topic];
}

bool IqWampSubscriptionsBase::hasSubscription(const QString &topic) const
{
    return m_subscriptionOnTopic.contains(topic);
}

bool IqWampSubscriptionsBase::hasSubscription(int id) const
{
    return m_subscriptionOnId.contains(id);
}

bool IqWampSubscriptionsBase::addSubscription(QSharedPointer<IqWampSubscription> &&subscription)
{
    if (subscription->topic().isEmpty())
        return false;
    m_subscriptionOnTopic[subscription->topic()] = subscription;

    if (subscription->id() != -1)
        m_subscriptionOnId[subscription->id()] = subscription;

    return true;
}

QSharedPointer<IqWampSubscription> IqWampSubscriptionsBase::subscription(int id) const
{
    return m_subscriptionOnId[id];
}

QSharedPointer<IqWampSubscription> IqWampSubscriptionsBase::take(int id)
{
    QSharedPointer<IqWampSubscription> result = subscription(id);
    removeSubscription(result);
    return result;
}

bool IqWampSubscriptionsBase::removeSubscription(QSharedPointer<IqWampSubscription> &subscription)
{
    m_subscriptionOnId.remove(subscription->id());
    m_subscriptionOnTopic.remove(subscription->topic());
    return true;
}






