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

#include "iqwampclient.h"
#include <QWebSocket>
#include "iqwamp.h"
#include "iqwampjsonwebsockethelper.h"
#include "iqwampcallbacksubscriptions.h"
#include "iqwampcallbackregistrations.h"

class IqWampClientPrivate : public QObject
{
    Q_DECLARE_PUBLIC(IqWampClient)
public:
    IqWampClientPrivate(IqWampClient *client);

    bool open(const QString &url, const QString &realm);

    bool subscribeToTopic(const QString &topic,
                          const QJsonObject &options,
                          std::function<void(const QJsonArray &, const QJsonObject &)> callback);

    bool callProcedure(const QString &procedure,
                       const QJsonArray &arguments,
                       const QJsonObject &argumentsKw,
                       std::function<void(const QJsonArray &, const QJsonObject &)> callback,
                       std::function<void(const IqWampCallError &error)> errorCallback);
private:
    int newMessageRequest();

    void processTextMessage(const QString &message);
    void sendHello();

    void processWelcome(const QJsonArray &jsonMessage);

    void processSubscribed(const QJsonArray &jsonMessage);

    void processError(const QJsonArray &jsonMessage);

    void processResult(const QJsonArray &jsonMessage);

private:
    QScopedPointer<QWebSocket> m_socket;
    IqWampClient *q_ptr;
    QString m_realm;
    int m_currentMessageId;
    bool m_opened;
    QString m_sessionId;
    IqWampCallbackSubscriptions *m_subscribedQuery;
    IqWampCallbackSubscriptions *m_subscriptions;
    IqWampCallbackRegistrations *m_registrations;

    void processEvent(const QJsonArray &jsonMessage);
    void processCallError (int requestId,
                           const QJsonObject &details,
                           const QString &error,
                           const QJsonArray &arguments,
                           const QJsonObject &argumentsKw);

    QHash<int, QSharedPointer<IqWampCallbackSubscription> > m_subscribedQuery;
    void processEvent(const QJsonArray &jsonMessage);
};

IqWampClientPrivate::IqWampClientPrivate(IqWampClient *client) :
    QObject(client),
    m_socket(new QWebSocket()),
    q_ptr(client),
    m_currentMessageId(0),
    m_opened(false),
    m_subscribedQuery(new IqWampCallbackSubscriptions(this)),
    m_subscriptions(new IqWampCallbackSubscriptions(this)),
    m_registrations(new IqWampCallbackRegistrations(this))
{
    connect(m_socket.data(), &QWebSocket::connected, this, &IqWampClientPrivate::sendHello);
    connect(m_socket.data(), &QWebSocket::textMessageReceived, this, &IqWampClientPrivate::processTextMessage);
}

bool IqWampClientPrivate::open(const QString &url, const QString &realm)
{
    m_realm = realm;
    m_opened = false;

    m_socket->open(url);

    return true;
}

int IqWampClientPrivate::newMessageRequest()
{
    return m_currentMessageId++;
}

void IqWampClientPrivate::sendHello()
{
    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::Hello));
    message.append(m_realm);

    QJsonObject details;
    QString libVersion = LIB_VERSION;
    details.insert("agent", QString("IqWampClient-%0").arg(libVersion));

    QJsonObject roles;
    roles.insert("publisher", QJsonObject());
    roles.insert("subscriber", QJsonObject());
    roles.insert("caller", QJsonObject());
    roles.insert("callee", QJsonObject());
    details.insert("roles", roles);
    message.append(details);

    IqWampJsonWebSocketHelper::send(m_socket.data(), message);
}

void IqWampClientPrivate::processTextMessage(const QString &message)
{
    QJsonArray messageArray = QJsonArray();
    IqWamp::MessageTypes messageType;

    if (!IqWampJsonWebSocketHelper::parseMessage(message, &messageArray, &messageType))
        return;

    switch (messageType) {
        case IqWamp::MessageTypes::Hello: {
#ifdef IQWAMP_DEBUG_MODE
            qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message can not be reserve from server!";
#endif
        }
            break;
        case IqWamp::MessageTypes::Welcome: {
            processWelcome(messageArray);
        } break;
        case IqWamp::MessageTypes::Event: {
            processEvent(messageArray);
        } break;
        case IqWamp::MessageTypes::Subscribed: {
            processSubscribed(messageArray);
        } break;
        case IqWamp::MessageTypes::Error: {
            processError(messageArray);
        } break;
        case IqWamp::MessageTypes::Result: {
            processResult(messageArray);
        } break;

//        case IqWamp::MessageTypes::UnSubscribe: {
//        }
//            break;
//        case IqWamp::MessageTypes::Publish: {
//            break;
//        }
//
//        case IqWamp::MessageTypes::Register: {
//            break;
//        }
//        case IqWamp::MessageTypes::UnRegister: {
//            break;
//        }
//        case IqWamp::MessageTypes::Call: {
//            break;
//        }

        default: {
#ifdef IQWAMP_DEBUG_MODE
            qDebug() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message temporary not support.";
#endif
        }
            break;
    }
}

void IqWampClientPrivate::processResult(const QJsonArray &jsonMessage)
{
    IqWamp::MessageTypes messageType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(0).toInt());

    if (!jsonMessage.at(static_cast<int>(IqWamp::ResultMessage::Request)).isDouble()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Request must be id.";
#endif
        return;
    }
    int requestId = jsonMessage.at(static_cast<int>(IqWamp::ResultMessage::Request)).toInt();

    if (!jsonMessage.at(static_cast<int>(IqWamp::ResultMessage::Details)).isObject()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Details must be dict.";
#endif
        return;
    }
    QJsonObject details = jsonMessage.at(static_cast<int>(IqWamp::ResultMessage::Details)).toObject();

    QJsonArray arguments;
    if (jsonMessage.size() > static_cast<int>(IqWamp::ResultMessage::Arguments)) {
        if (!jsonMessage.at(static_cast<int>(IqWamp::ResultMessage::Arguments)).isArray()) {
#ifdef IQWAMP_DEBUG_MODE
            qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Arguments must be list.";
#endif
            return;
        }
        arguments = jsonMessage.at(static_cast<int>(IqWamp::ResultMessage::Arguments)).toArray();
    }

    QJsonObject argumentsKw;
    if (jsonMessage.size() > static_cast<int>(IqWamp::ResultMessage::ArgumentsKw)) {
        if (!jsonMessage.at(static_cast<int>(IqWamp::ResultMessage::ArgumentsKw)).isObject()) {
#ifdef IQWAMP_DEBUG_MODE
            qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! ArgumentsKw must be list.";
#endif
            return;
        }
        argumentsKw = jsonMessage.at(static_cast<int>(IqWamp::ResultMessage::Arguments)).toObject();
    }

    QSharedPointer<IqWampCallbackRegistration> registration = m_registrations->take(requestId);

    registration->execute(arguments, argumentsKw);
}


void IqWampClientPrivate::processError(const QJsonArray &jsonMessage)
{
    IqWamp::MessageTypes messageType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(0).toInt());

    if (!jsonMessage.at(static_cast<int>(IqWamp::ErrorMessage::Call)).isDouble()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! CALL must be id.";
#endif
        return;
    }
    IqWamp::MessageTypes callType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(static_cast<int>(IqWamp::ErrorMessage::Call)).toInt());

    if (!jsonMessage.at(static_cast<int>(IqWamp::ErrorMessage::Request)).isDouble()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Request must be id.";
#endif
        return;
    }
    int requestId = jsonMessage.at(static_cast<int>(IqWamp::ErrorMessage::Request)).toInt();

    if (!jsonMessage.at(static_cast<int>(IqWamp::ErrorMessage::Details)).isObject()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Details must be dict.";
#endif
        return;
    }
    QJsonObject details = jsonMessage.at(static_cast<int>(IqWamp::ErrorMessage::Details)).toObject();

    if (!jsonMessage.at(static_cast<int>(IqWamp::ErrorMessage::Error)).isString()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Error must be uri.";
#endif
        return;
    }
    QString error = jsonMessage.at(static_cast<int>(IqWamp::ErrorMessage::Error)).toString();

    QJsonArray arguments;
    if (jsonMessage.size() > static_cast<int>(IqWamp::ErrorMessage::Arguments)) {
        if (!jsonMessage.at(static_cast<int>(IqWamp::ErrorMessage::Arguments)).isArray()) {
#ifdef IQWAMP_DEBUG_MODE
            qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Arguments must be list.";
#endif
            return;
        }
        arguments = jsonMessage.at(static_cast<int>(IqWamp::ErrorMessage::Arguments)).toArray();
    }

    QJsonObject argumentsKw;
    if (jsonMessage.size() > static_cast<int>(IqWamp::ErrorMessage::ArgumentsKw)) {
        if (!jsonMessage.at(static_cast<int>(IqWamp::ErrorMessage::ArgumentsKw)).isObject()) {
#ifdef IQWAMP_DEBUG_MODE
            qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! ArgumentsKw must be list.";
#endif
            return;
        }
        argumentsKw = jsonMessage.at(static_cast<int>(IqWamp::ErrorMessage::Arguments)).toObject();
    }

    switch (callType) {
        case IqWamp::MessageTypes::Call: {
            processCallError(requestId, details, error, arguments, argumentsKw);
        } break;
        default: {
#ifdef IQWAMP_DEBUG_MODE
            qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message with id =" << requestId << "is not expected!";
#endif
            return;
        }
    }
}

void IqWampClientPrivate::processWelcome(const QJsonArray &jsonMessage)
{
    IqWamp::MessageTypes messageType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(0).toInt());

    if (m_opened) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << "Reserved extra" << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message! Connection already opened.";
#endif
        return;
    }

    QJsonValue session = jsonMessage.at(1);

    if (!session.isString()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Session must be id.";
#endif
        return;
    }
    m_sessionId = session.toString();

    m_opened = true;

    Q_Q(IqWampClient);
    emit q->opened();
}

bool IqWampClientPrivate::subscribeToTopic(const QString &topic,
                                           const QJsonObject &options,
                                           std::function<void(const QJsonArray &, const QJsonObject &)> callback)
{
    int requestId = newMessageRequest();
    m_subscribedQuery->create(requestId, topic, callback);
    QSharedPointer<IqWampCallbackSubscription> subscription = QSharedPointer<IqWampCallbackSubscription>::create(-1, topic, callback);
    int requestId = newMessageRequest();
    m_subscribedQuery[requestId] = subscription;

    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::Subscribe));
    message.append(requestId);
    message.append(options);
    message.append(topic);

    IqWampJsonWebSocketHelper::send(m_socket.data(), message);

    return true;
}

void IqWampClientPrivate::processSubscribed(const QJsonArray &jsonMessage)
{
    IqWamp::MessageTypes messageType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(0).toInt());

    if (!jsonMessage.at(static_cast<int>(IqWamp::SubscribedMessage::Request)).isDouble()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Request must be id.";
#endif
        return;
    }

    int requestId = jsonMessage.at(static_cast<int>(IqWamp::SubscribedMessage::Request)).toInt();

    if (!jsonMessage.at(static_cast<int>(IqWamp::SubscribedMessage::Subscription)).isDouble()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Subscription must be id.";
#endif
        return;
    }

    int subscriptionId = jsonMessage.at(static_cast<int>(IqWamp::SubscribedMessage::Subscription)).toInt();

    if (!m_subscribedQuery->hasSubscription(requestId)) {
    if (!m_subscribedQuery.contains(requestId)) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message with id =" << requestId << "is not expected!";
#endif
        return;
    }

    QSharedPointer<IqWampCallbackSubscription> subscription = m_subscribedQuery->take(requestId);
    QSharedPointer<IqWampCallbackSubscription> subscription = m_subscribedQuery.take(requestId);
    m_subscriptions->create(subscriptionId, subscription->topic(), subscription->callback());
}

void IqWampClientPrivate::processEvent(const QJsonArray &jsonMessage)
{

    IqWamp::MessageTypes messageType = static_cast<IqWamp::MessageTypes>(jsonMessage.at(0).toInt());

    if (!jsonMessage.at(static_cast<int>(IqWamp::EventMessage::Subscription)).isDouble()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Subscription must be id.";
#endif
        return;
    }

    int subscriptionId = jsonMessage.at(static_cast<int>(IqWamp::EventMessage::Subscription)).toInt();

    if (!jsonMessage.at(static_cast<int>(IqWamp::EventMessage::Publication)).isDouble()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Publication must be id.";
#endif
        return;
    }

//    int publicationId = jsonMessage.at(static_cast<int>(IqWamp::EventMessage::Publication)).toInt();

    if (!jsonMessage.at(static_cast<int>(IqWamp::EventMessage::Details)).isObject()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Details must be dict.";
#endif
        return;
    }

    QJsonArray arguments = QJsonArray();
    if (jsonMessage.size() > static_cast<int>(IqWamp::EventMessage::Arguments) ) {
        if (!jsonMessage.at(static_cast<int>(IqWamp::EventMessage::Arguments)).isArray()) {
#ifdef IQWAMP_DEBUG_MODE
            qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! Arguments must be list.";
#endif
            return;
        }

        arguments = jsonMessage.at(static_cast<int>(IqWamp::EventMessage::Arguments)).toArray();
    }

    QJsonObject argumentsKw = QJsonObject();
    if (jsonMessage.size() > static_cast<int>(IqWamp::EventMessage::ArgumentsKw) ) {
        if (!jsonMessage.at(static_cast<int>(IqWamp::EventMessage::ArgumentsKw)).isObject()) {
#ifdef IQWAMP_DEBUG_MODE
            qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not formatted correctly! ArgumentsKw must be dict.";
#endif
            return;
        }

        argumentsKw = jsonMessage.at(static_cast<int>(IqWamp::EventMessage::ArgumentsKw)).toObject();
    }

    if (!m_subscriptions->hasSubscription(subscriptionId)) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message is not expected! Not subscribed to id =" << subscriptionId << ".";
#endif
        return;
    }

    QSharedPointer<IqWampCallbackSubscription> subscription = m_subscriptions->subscription(subscriptionId);
    subscription->execute(arguments, argumentsKw);
}

bool IqWampClientPrivate::callProcedure(const QString &procedure,
                                        const QJsonArray &arguments,
                                        const QJsonObject &argumentsKw,
                                        std::function<void(const QJsonArray &, const QJsonObject &)> callback,
                                        std::function<void(const IqWampCallError &error)> errorCallback)
{
    int requestId = newMessageRequest();
    m_registrations->create(requestId, procedure, callback, errorCallback);

    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::Call));
    message.append(requestId);
    message.append(QJsonObject());
    message.append(procedure);
    message.append(arguments);
    message.append(argumentsKw);

    IqWampJsonWebSocketHelper::send(m_socket.data(), message);

    return true;
}

void IqWampClientPrivate::processCallError(int requestId,
                                           const QJsonObject &details,
                                           const QString &error,
                                           const QJsonArray &arguments,
                                           const QJsonObject &argumentsKw)
{
    if (!m_registrations->hasRegistration(requestId)) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(IqWamp::MessageTypes::Error) << "message with id =" << requestId << "is not expected!";
#endif
        return;
    }

    QSharedPointer<IqWampCallbackRegistration> registration = m_registrations->take(requestId);

    IqWampCallError callError (details, error, arguments, argumentsKw);

    registration->executeError(callError);
}

IqWampClient::IqWampClient(QObject *parent) :
    IqWampAbstractClient(parent),
    d_ptr(new IqWampClientPrivate(this))
{
}

bool IqWampClient::subscribeToTopic(const QString &topic,
                                    const QJsonObject &options,
                                    std::function<void(const QJsonArray &, const QJsonObject &)> callback)
{
    Q_D(IqWampClient);
    return d->subscribeToTopic(topic, options, callback);
}

bool IqWampClient::publishEvent(const QString &topic, const QJsonArray &arguments, const QJsonObject &argumentsKw)
{
    return false;
}

bool IqWampClient::registerProcedureCallback(const QString &procedure,
                                             std::function<IqWampYieldResult(const QJsonArray &,
                                                                             const QJsonObject &)> callback)
{
    return false;
}

bool IqWampClient::open(const QString &url, const QString &realm)
{
    Q_D(IqWampClient);
    return d->open(url, realm);
}

bool IqWampClient::callProcedure(const QString &procedure,
                                 const QJsonArray &arguments,
                                 const QJsonObject &argumentsKw,
                                 std::function<void(const QJsonArray &, const QJsonObject &)> callback,
                                 std::function<void(const IqWampCallError &error)> errorCallback)
{
    Q_D(IqWampClient);
    return d->callProcedure(procedure, arguments, argumentsKw, callback, errorCallback);
}
