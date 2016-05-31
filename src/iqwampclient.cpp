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

class IqWampClientPrivate : public QObject
{
    Q_DECLARE_PUBLIC(IqWampClient)
public:
    IqWampClientPrivate(IqWampClient *client);

    bool open(const QString &url, const QString &realm);

private:
    QJsonValue newMessageRequest();

    void processTextMessage(const QString &message);
    void sendHello();

    void processWelcome(const QJsonArray &jsonMessage);
private:
    QScopedPointer<QWebSocket> m_socket;
    IqWampClient *q_ptr;
    QString m_realm;
    int m_currentMessageId;
    bool m_opened;
    QString m_sessionId;
};

IqWampClientPrivate::IqWampClientPrivate(IqWampClient *client) :
    QObject(client),
    m_socket(new QWebSocket()),
    q_ptr(client),
    m_realm(""),
    m_currentMessageId(0),
    m_opened(false),
    m_sessionId("")
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

QJsonValue IqWampClientPrivate::newMessageRequest()
{
    return QJsonValue(m_currentMessageId++);
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
        }
            break;

        case IqWamp::MessageTypes::Subscribe: {
        }
            break;
        case IqWamp::MessageTypes::UnSubscribe: {
        }
            break;
        case IqWamp::MessageTypes::Publish: {
            break;
        }

        case IqWamp::MessageTypes::Register: {
            break;
        }
        case IqWamp::MessageTypes::UnRegister: {
            break;
        }
        case IqWamp::MessageTypes::Call: {
            break;
        }

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







IqWampClient::IqWampClient(QObject *parent) :
    IqWampAbstractClient(parent),
    d_ptr(new IqWampClientPrivate(this))
{
}

bool IqWampClient::subscribeToTopic(const QString &topic,
                                    const QJsonObject &options,
                                    std::function<void(const QJsonArray &, const QJsonObject &)> callback)
{
    return true;
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

