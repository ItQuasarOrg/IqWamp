/**********************************************************************************
 * Copyright © 2016 Pavel A. Puchkov                                              *
 *                                                                                *
 * This file is part of IqWamp.                                                   *
 *                                                                                *
 * IqWamp is free software: you can redistribute it and/or modify                 *
 * it under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or              *
 * (at your option) any later version.                                            *
 *                                                                                *
 * IqWamp is distributed in the hope that it will be useful,                      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 * GNU Lesser General Public License for more details.                            *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public License       *
 * along with IqWamp.  If not, see <http://www.gnu.org/licenses/>.                *
 **********************************************************************************/

#include "iqwampcallee.h"
#include "iqwamprealm.h"
#include "iqwampjsonwebsockethelper.h"
#include <QUuid>
#include <QJsonDocument>


IqWampCallee::IqWampCallee(QWebSocket *socket, QObject *parent):
    IqWampAbstractCallee(parent),
    m_socket(socket)
{
    socket->setParent(this);
    connect(socket, &QWebSocket::disconnected, this, &IqWampCallee::disconnected);
    connect(socket, &QWebSocket::textMessageReceived, this, &IqWampCallee::processTextMessage);
}

IqWampCallee::~IqWampCallee()
{

}

void IqWampCallee::processTextMessage(const QString &message)
{
    QJsonArray messageArray = QJsonArray();
    IqWamp::MessageTypes messageType;

    if (!IqWampJsonWebSocketHelper::parseMessage(message, &messageArray, &messageType))
        return;

    switch (messageType) {
        case IqWamp::MessageTypes::Hello: {
            processHello(messageArray);
        }
            break;
        case IqWamp::MessageTypes::Welcome:
        case IqWamp::MessageTypes::Event: {
#ifdef IQWAMP_DEBUG_MODE
            qWarning() << IqWampCallee::messageTypeName(messageType) << "message can not be reserve from client!";
#endif
        }
            break;

        case IqWamp::MessageTypes::Subscribe: {
            processSubscribe(messageArray);
        }
            break;
        case IqWamp::MessageTypes::UnSubscribe: {
            processUnSubscribe(messageArray);
        }
            break;
        case IqWamp::MessageTypes::Publish: {
            processPublish(messageArray);
            break;
        }

        case IqWamp::MessageTypes::Register: {
            processRegister(messageArray);
            break;
        }
        case IqWamp::MessageTypes::UnRegister: {
            processUnRegister(messageArray);
            break;
        }
        case IqWamp::MessageTypes::Call: {
            processCall(messageArray);
            break;
        }

        default: {
#ifdef IQWAMP_DEBUG_MODE
            qDebug() << IqWampCallee::messageTypeName(messageType) << "message temporary not support.";
#endif
        }
            break;
    }
}

void IqWampCallee::processHello(const QJsonArray &jsonMessage)
{
    IqWamp::MessageTypes messageType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(0).toInt());

    QJsonValue messageSecondParam = jsonMessage.at(1);

    if (!messageSecondParam.isString()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampCallee::messageTypeName(messageType) << "message is not formatted correctly! Second value on message array must be string.";
#endif
        return;
    }
    emit hello(messageSecondParam.toString());
}

QString IqWampCallee::messageTypeName(IqWamp::MessageTypes messageType)
{
    return IqWampJsonWebSocketHelper::messageTypeName(messageType);
}

void IqWampCallee::sendEvent(const QSharedPointer<IqWampSubscription> &subscription,
                             int publicationId,
                             const QJsonArray &arguments,
                             const QJsonObject &argumentsKw)
{
    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::Event));
    message.append(subscription->id());
    message.append(publicationId);
    message.append(QJsonObject());
    if (!arguments.isEmpty() || !argumentsKw.isEmpty())
        message.append(arguments);
    if (!argumentsKw.isEmpty())
    message.append(argumentsKw);

    send(message);
}

QString IqWampCallee::sessionId() const
{
    return m_sessionId;
}

void IqWampCallee::sendWelcome()
{
    createSessionId();

    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::Welcome));
    message.append(QJsonValue(m_sessionId));

    QJsonObject details;
    QString libVersion = LIB_VERSION;
    details.insert("agent", QString("IqWampServer-%0").arg(libVersion));

    QJsonObject roles;
    roles.insert("publisher", QJsonObject());
    roles.insert("subscriber", QJsonObject());
    roles.insert("caller", QJsonObject());
    roles.insert("callee", QJsonObject());
    details.insert("roles", roles);
    message.append(details);

    send(message);
}

void IqWampCallee::createSessionId()
{
    m_sessionId = QUuid::createUuid().toString();
}

void IqWampCallee::send(const QJsonArray &jsonArray)
{
    IqWampJsonWebSocketHelper::send(m_socket, jsonArray);
}

void IqWampCallee::sendAbort(const QString &reason, const QJsonObject &details)
{
    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::Abort));
    message.append(details);
    message.append(reason);

    send(message);
}

void IqWampCallee::closeConnection()
{
    m_socket->close();
}

void IqWampCallee::processRegister(const QJsonArray &jsonMessage)
{
    IqWamp::MessageTypes messageType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(0).toInt());
    QJsonValue request = jsonMessage.at(1);

    if (!jsonMessage.at(3).isString()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampCallee::messageTypeName(messageType) << "message is not formatted correctly! Procedure must be url.";
#endif
        return;
    }

    QString procedure = jsonMessage.at(3).toString();

    IqWampRegistrations *registrations = m_realm->registrations();

    if (registrations->hasRegistration(procedure)) {
        QSharedPointer<IqWampRegistration> registration = registrations->registration(procedure);
        if (registration->callee() != this) {
            sendError(messageType, request, IqWamp::Errors::ProcedureAlreadyExists);
            return;
        } else {
            sendRegistered(request, registration->id());
            return;
        }
    }

    QSharedPointer<IqWampRegistration> registration = registrations->create(procedure, this);

    sendRegistered(request, registration->id());
}

void IqWampCallee::processSubscribe(const QJsonArray &jsonMessage)
{
    IqWamp::MessageTypes messageType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(0).toInt());
    QJsonValue request = jsonMessage.at(1);

    if (!jsonMessage.at(3).isString()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampCallee::messageTypeName(messageType) << "message is not formatted correctly! Topic must be uri.";
#endif
        return;
    }

    QString topic = jsonMessage.at(3).toString();

    IqWampSubscriptions *subscriptions = m_realm->subscriptions();
    QSharedPointer<IqWampSubscription> subscription;

    if (subscriptions->hasSubscription(topic)) {
         subscription = subscriptions->subscription(topic);

        if (!subscription->hasCallee(this))
            subscription->addCallee(this);
    } else {
        subscription = subscriptions->create(topic, this);
    }

    sendSubscribed(request, subscription->id());
}

void IqWampCallee::sendSubscribed(const QJsonValue &request, int subscriptionId)
{
    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::Subscribed));
    message.append(request);
    message.append(subscriptionId);

    send(message);
}

void IqWampCallee::sendRegistered(const QJsonValue &request, int registrationId)
{
    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::Registered));
    message.append(request);
    message.append(registrationId);

    send(message);
}

void IqWampCallee::processUnRegister(const QJsonArray &jsonMessage)
{
    IqWamp::MessageTypes messageType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(0).toInt());
    QJsonValue request = jsonMessage.at(1);

    if (!jsonMessage.at(2).isDouble()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampCallee::messageTypeName(messageType) << "message is not formatted correctly! REGISTRATION.Registration must be id.";
#endif
        return;
    }

    int registrationId = jsonMessage.at(2).toInt();

    IqWampRegistrations *registrations = m_realm->registrations();

    if (!registrations->hasRegistration(registrationId)) {
        sendError(messageType, request, IqWamp::Errors::NoSuchRegistration);
        return;
    } else {
        QSharedPointer<IqWampRegistration> registration = registrations->registration(registrationId);
        if (registration->callee() != this) {
            sendError(messageType, request, IqWamp::Errors::NotOwner);
            return;
        }
    }

    registrations->remove(registrationId);

    sendUnRegistered(request);
}

void IqWampCallee::processUnSubscribe(const QJsonArray &jsonMessage)
{
    IqWamp::MessageTypes messageType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(0).toInt());
    QJsonValue request = jsonMessage.at(1);

    if (!jsonMessage.at(2).isDouble()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampCallee::messageTypeName(messageType) << "message is not formatted correctly! SUBSCRIBED.Subscription must be id.";
#endif
        return;
    }

    int subscriptionId = jsonMessage.at(2).toInt();

    IqWampSubscriptions *subscriptions = m_realm->subscriptions();

    if (!subscriptions->hasSubscription(subscriptionId)) {
        sendError(messageType, request, IqWamp::Errors::NoSuchSubscription);
        return;
    } else {
        QSharedPointer<IqWampSubscription> subscription = subscriptions->subscription(subscriptionId);
        if (!subscription->hasCallee(this)) {
            sendError(messageType, request, IqWamp::Errors::NotSubscribed);
            return;
        }
        subscription->removeCallee(this);
    }

    sendUnSubscribed(request);
}

void IqWampCallee::sendUnSubscribed(const QJsonValue &request)
{
    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::UnSubscribed));
    message.append(request);

    send(message);
}

void IqWampCallee::sendUnRegistered(const QJsonValue &request)
{
    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::UnRegistered));
    message.append(request);

    send(message);
}

void IqWampCallee::sendError(IqWamp::MessageTypes requestType,
                                   const QJsonValue &request,
                                   const QString &error,
                                   const QJsonObject &details)
{
    IqWampJsonWebSocketHelper::sendError(m_socket, requestType, request, error, details);
}

void IqWampCallee::processCall(const QJsonArray &jsonMessage)
{
    IqWamp::MessageTypes messageType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(0).toInt());
    QJsonValue request = jsonMessage.at(1);

    if (!jsonMessage.at(3).isString()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampCallee::messageTypeName(messageType) << "message is not formatted correctly! Procedure must be uri.";
#endif
        return;
    }

    QJsonArray  arguments;
    QJsonObject  argumentsKw;
    if (jsonMessage.size() > 4) {
        if (!jsonMessage.at(4).isArray()) {
#ifdef IQWAMP_DEBUG_MODE
            qWarning() << IqWampCallee::messageTypeName(messageType) << "message is not formatted correctly! Arguments must be list.";
#endif
            return;
        }
        arguments = jsonMessage.at(4).toArray();

        if (jsonMessage.size() > 5) {
            if (!jsonMessage.at(5).isObject()) {
#ifdef IQWAMP_DEBUG_MODE
                qWarning() << IqWampCallee::messageTypeName(messageType) << "message is not formatted correctly! ArgumentsKw must be dict.";
#endif
                return;
            }
            argumentsKw = jsonMessage.at(5).toObject();
        }
    }

    QString procedure = jsonMessage.at(3).toString();

    IqWampRegistrations *registrations = m_realm->registrations();
    if (!registrations->hasRegistration(procedure)) {
        sendError(IqWamp::MessageTypes::Call, request, IqWamp::Errors::NoSuchProcedure);

        return;
    }

    QSharedPointer<IqWampRegistration> registration = registrations->registration(procedure);

    int callId = m_realm->dialer()->call(registration, arguments, argumentsKw, this);

    m_callRequests[callId] = request;
}

void IqWampCallee::processPublish(const QJsonArray &jsonMessage)
{
    IqWamp::MessageTypes messageType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(0).toInt());
    QJsonValue request = jsonMessage.at(1);

    if (!jsonMessage.at(3).isString()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampCallee::messageTypeName(messageType) << "message is not formatted correctly! Topic must be uri.";
#endif
        return;
    }

    QJsonArray  arguments;
    QJsonObject  argumentsKw;
    if (jsonMessage.size() > 4) {
        if (!jsonMessage.at(4).isArray()) {
#ifdef IQWAMP_DEBUG_MODE
            qWarning() << IqWampCallee::messageTypeName(messageType) << "message is not formatted correctly! Arguments must be list.";
#endif
            return;
        }
        arguments = jsonMessage.at(4).toArray();

        if (jsonMessage.size() > 5) {
            if (!jsonMessage.at(5).isObject()) {
#ifdef IQWAMP_DEBUG_MODE
                qWarning() << IqWampCallee::messageTypeName(messageType) << "message is not formatted correctly! ArgumentsKw must be dict.";
#endif
                return;
            }
            argumentsKw = jsonMessage.at(4).toObject();
        }
    }

    QString topic = jsonMessage.at(3).toString();
    IqWampSubscriptions *subscriptions = m_realm->subscriptions();
    if (!subscriptions->hasSubscription(topic)) {
        sendError(IqWamp::MessageTypes::Publish, request, IqWamp::Errors::NotFoundTopic);
        return;
    }

    QSharedPointer<IqWampSubscription> subscription = subscriptions->subscription(topic);

    int publicationId = m_realm->broker()->publish(subscription, arguments, argumentsKw);

    sendPublished(request, publicationId);
}

void IqWampCallee::sendPublished(const QJsonValue &request, int publicationId)
{
    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::Published));
    message.append(request);
    message.append(publicationId);

    send(message);
}

void IqWampCallee::setRealm(IqWampRealm *realm)
{
    m_realm = realm;
}

void IqWampCallee::sendInvocation(const QSharedPointer<IqWampRegistration> &registration,
                                  int invocationId,
                                  const QJsonArray &arguments,
                                  const QJsonObject &argumentsKw)
{

}

void IqWampCallee::sendResult(int callId,
                              const QJsonObject &details,
                              const QJsonArray &arguments,
                              const QJsonObject &argumentsKw)
{
    if(!m_callRequests.contains(callId))
        return;

    QJsonValue request = m_callRequests.take(callId);
    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::Result));
    message.append(request);
    message.append(details);
    if (!arguments.isEmpty() || !argumentsKw.isEmpty())
        message.append(arguments);
    if (!argumentsKw.isEmpty())
        message.append(argumentsKw);

    send(message);
}

void IqWampCallee::sendCallError(int callId, const QString &error, const QJsonObject &details)
{
    if(!m_callRequests.contains(callId))
        return;

    QJsonValue request = m_callRequests.take(callId);

    sendError(IqWamp::MessageTypes::Call, request, error, details);
}



