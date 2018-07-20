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
#include "iqwamplocalcallee.h"

IqWampLocalClient::IqWampLocalClient(QObject *parent, IqWampRealm *realm) :
    IqWampAbstractClient(parent),
    m_realm(realm),
    m_callee(new IqWampLocalCallee(this))
{
}

bool IqWampLocalClient::publishEvent(const QString &topic, const QJsonArray &arguments, const QJsonObject &argumentsKw)
{
    IqWampCalleeSubscriptions *subscriptions = m_realm->subscriptions();
    if (!subscriptions->hasSubscription(topic))
        return false;

    QSharedPointer<IqWampCalleeSubscription> subscription = subscriptions->subscription(topic);

    IqWampBroker *broker = m_realm->broker();
    broker->publish(subscription, arguments, argumentsKw);

    return true;
}

IqWampRealm *IqWampLocalClient::realm() const
{
    return m_realm;
}

bool IqWampLocalClient::registerProcedureCallback(const QString &procedure,
                                                  std::function<IqWampYieldResult(const QJsonArray &,
                                                                                  const QJsonObject &)> callback)
{
    IqWampCalleeRegistrations *registrations = m_realm->registrations();
    if (registrations->hasRegistration(procedure))
        return false;

//    QJsonArray jsonArray;
//    QJsonObject jsonObject;

    if (!m_callee->registerProcedure(procedure, callback))
        return false;

    return registrations->create(procedure, m_callee).data() != Q_NULLPTR;
}

bool IqWampLocalClient::subscribeToTopic(const QString &topic,
                                         const QJsonObject &options,
                                         std::function<void(const QJsonArray &, const QJsonObject &)> callback)
{
    return false;
}

bool IqWampLocalClient::callProcedure(const QString &procedure,
                                      const QJsonArray &arguments,
                                      const QJsonObject &argumentsKw,
                                      std::function<void(const QJsonArray &, const QJsonObject &)> callback,
                                      std::function<void(const IqWampCallError &error)> errorCallback)
{
    return false;
}



