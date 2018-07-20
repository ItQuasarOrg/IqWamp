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

#include "iqwamplocalcallee.h"
#include "iqwamplocalclient.h"
#include "iqwamprealm.h"

IqWampLocalCallee::IqWampLocalCallee(IqWampLocalClient *client, QObject *parent) :
    IqWampAbstractCallee(parent),
    m_localClient(client)
{
}

bool IqWampLocalCallee::registerProcedure(const QString &procedure, std::function<IqWampYieldResult(const QJsonArray &, const QJsonObject &)> callback)
{
    if (m_callbacks.contains(procedure))
        return false;

    m_callbacks[procedure] = callback;

    return true;
}

void IqWampLocalCallee::sendInvocation(const QSharedPointer<IqWampCalleeRegistration> &registration,
                                       int invocationId,
                                       const QJsonArray &arguments,
                                       const QJsonObject &argumentsKw)
{
    QString procedure = registration->procedure();

    Q_ASSERT(m_callbacks.contains(procedure));

    std::function<IqWampYieldResult(const QJsonArray &, const QJsonObject &)> callback = m_callbacks[procedure];

    IqWampYieldResult callbackResult = callback(arguments, argumentsKw);
    callbackResult.setId(invocationId);

    IqWampDialer *dialer = m_localClient->realm()->dialer();

    dialer->processYield(registration, callbackResult);
}

void IqWampLocalCallee::sendEvent(const QSharedPointer<IqWampCalleeSubscription> &subscription,
                                  int publicationId,
                                  const QJsonArray &arguments,
                                  const QJsonObject &argumentsKw)
{
    emit event(subscription->topic(), publicationId, arguments, argumentsKw);
}

bool IqWampLocalCallee::subscribe(const QString &topic)
{
    IqWampCalleeSubscriptions *subscriptions = m_localClient->realm()->subscriptions();
    QSharedPointer<IqWampCalleeSubscription> subscription;

    if (subscriptions->hasSubscription(topic)) {
        subscription = subscriptions->subscription(topic);

        if (!subscription->hasCallee(this))
            subscription->addCallee(this);
    } else {
        subscriptions->create(topic, this);
    }

    return true;
}

void IqWampLocalCallee::sendResult(int callId,
                                   const QJsonObject &details,
                                   const QJsonArray &arguments,
                                   const QJsonObject &argumentsKw)
{
    emit result(callId, details, arguments, argumentsKw);
}

void IqWampLocalCallee::sendCallError(int callId,
                                      const QString &error,
                                      const QJsonObject &details)
{
    emit callError(callId, error, details);
}


