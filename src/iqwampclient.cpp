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

class IqWampClientPrivate : public QObject
{
    Q_DECLARE_PUBLIC(IqWampClient)
public:
    IqWampClientPrivate(IqWampClient *client);

    bool open(const QString &url, const QString &realm);

    bool subscribeToTopic(const QString &topic,
                          const QJsonObject &options,
                          std::function<void(const QJsonArray &, const QJsonObject &)> callback);

private:
    int newMessageRequest();

    void processTextMessage(const QString &message);
    void sendHello();

    void processWelcome(const QJsonArray &jsonMessage);

    void processSubscribed(const QJsonArray &jsonMessage);
private:
    QScopedPointer<QWebSocket> m_socket;
    IqWampClient *q_ptr;
    QString m_realm;
    int m_currentMessageId;
    bool m_opened;
    QString m_sessionId;
    IqWampCallbackSubscriptions *m_subscriptions;

    QHash<int, QSharedPointer<IqWampCallbackSubscription> > m_subscribedQuery;
    void processEvent(const QJsonArray &jsonMessage);
};

IqWampClientPrivate::IqWampClientPrivate(IqWampClient *client) :
    QObject(client),
    m_socket(new QWebSocket()),
    q_ptr(client),
    m_currentMessageId(0),
    m_opened(false),
    m_subscriptions(new IqWampCallbackSubscriptions(this))
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

    if (!m_subscribedQuery.contains(requestId)) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << IqWampJsonWebSocketHelper::messageTypeName(messageType) << "message with id =" << requestId << "is not expected!";
#endif
        return;
    }

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

