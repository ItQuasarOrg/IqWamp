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

#define DEFAULT_CALL_IDLE_INTERVAL 3000

#include "iqwampdialer.h"
#include "iqwampabstractcallee.h"
#include "iqwampcallee.h"

IqWampDialer::IqWampInvocationFuture::IqWampInvocationFuture():
    invocationId(0),
    sender(Q_NULLPTR),
    idleTimer(new QTimer())
{
}

IqWampDialer::IqWampInvocationFuture::IqWampInvocationFuture(int invocationId, IqWampAbstractCallee *sender):
    invocationId(invocationId),
    sender(sender),
    idleTimer(new QTimer())
{
}

IqWampDialer::IqWampDialer(QObject *parent):
    QObject(parent),
    m_lastInvocationId(0),
    m_callIdleInterval(DEFAULT_CALL_IDLE_INTERVAL)
{
}

int IqWampDialer::call(const QSharedPointer<IqWampCalleeRegistration> &registration,
                       const QJsonArray &arguments,
                       const QJsonObject &argumentsKw,
                       IqWampAbstractCallee *sender)
{
    int invocationId = m_lastInvocationId++;

    IqWampInvocationFuture callFuture (invocationId, sender);
    connect(callFuture.idleTimer.data(), &QTimer::timeout, this, [callFuture, this](){
        m_invocationFutures.remove(callFuture.invocationId);

        if (callFuture.sender)
            callFuture.sender->sendCallError(callFuture.invocationId, IqWamp::Errors::Timeout);
    });
    m_invocationFutures[invocationId] = callFuture;
    callFuture.idleTimer->start(m_callIdleInterval);

    IqWampAbstractCallee *callee = registration->callee();

    QMetaObject::invokeMethod(this,
                              "sendInvocation",
                              Qt::QueuedConnection,
                              Q_ARG(QPointer<IqWampAbstractCallee>, QPointer<IqWampAbstractCallee>(callee)),
                              Q_ARG(const QSharedPointer<IqWampCalleeRegistration> &, registration),
                              Q_ARG(int, invocationId),
                              Q_ARG(const QJsonArray &, arguments),
                              Q_ARG(const QJsonObject &, argumentsKw));

    return invocationId;
}

void IqWampDialer::processYield(const QSharedPointer<IqWampCalleeRegistration> &registration,
                                const IqWampYieldResult &result)
{
    if (!m_invocationFutures.contains(result.id()))
        return;

    IqWampInvocationFuture callFuture = m_invocationFutures.take(result.id());
    callFuture.idleTimer->stop();

    if (callFuture.sender)
        callFuture.sender->sendResult(result.id(), QJsonObject(), result.arguments(), result.argumentsKw());
}

void IqWampDialer::sendInvocation(QPointer<IqWampAbstractCallee> callee,
                                  const QSharedPointer<IqWampCalleeRegistration> &registration,
                                  int invocationId,
                                  const QJsonArray &arguments,
                                  const QJsonObject &argumentsKw)
{
    callee->sendInvocation(registration, invocationId, arguments, argumentsKw);
}



