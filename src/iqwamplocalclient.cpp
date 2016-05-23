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

#include "iqwamplocalclient.h"
#include "iqwamprealm.h"

IqWampLocalClient::IqWampLocalClient(QObject *parent, IqWampRealm *realm) :
    QObject(parent),
    m_realm(realm)
{
}

bool IqWampLocalClient::publish(const QString &topic, const QJsonObject &argumentsKw)
{
    return publish(topic, QJsonArray(), argumentsKw);
}

bool IqWampLocalClient::publish(const QString &topic, const QJsonArray &arguments)
{
    return publish(topic, arguments, QJsonObject());
}

bool IqWampLocalClient::publish(const QString &topic, const QJsonArray &arguments, const QJsonObject &argumentsKw)
{
    IqWampSubscriptions *subscriptions = m_realm->subscriptions();
    if (!subscriptions->hasSubscription(topic))
        return false;

    QSharedPointer<IqWampSubscription> subscription = subscriptions->subscription(topic);

    IqWampBroker *broker = m_realm->broker();
    broker->publish(subscription, arguments, argumentsKw);

    return true;
}

