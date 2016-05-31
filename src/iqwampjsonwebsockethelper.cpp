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

#include "iqwampjsonwebsockethelper.h"
#include <QJsonDocument>

void IqWampJsonWebSocketHelper::sendError(QWebSocket *socket,
                                 IqWamp::MessageTypes requestType,
                                 const QJsonValue &request,
                                 const QString &error,
                                 const QJsonObject &details)
{
    QJsonArray message;
    message.append(static_cast<int>(IqWamp::MessageTypes::Error));
    message.append(static_cast<int>(requestType));
    message.append(request);
    message.append(details);
    message.append(error);

    send(socket, message);
}

void IqWampJsonWebSocketHelper::send(QWebSocket *socket, const QJsonArray &jsonArray)
{
    QJsonDocument doc (jsonArray);

    QJsonDocument::JsonFormat format = QJsonDocument::Compact;
#ifdef IQWAMP_DEBUG_MODE
    format = QJsonDocument::Indented;
#endif

    QString message = doc.toJson(format);

#ifdef IQWAMP_DEBUG_MODE
    qDebug() << "Send message" << message;
#endif

    socket->sendTextMessage(message);
}

bool IqWampJsonWebSocketHelper::parseMessage(const QString &message,
                                             QJsonArray *jsonMessage,
                                             IqWamp::MessageTypes *messageType)
{
    Q_CHECK_PTR(jsonMessage);
    Q_CHECK_PTR(messageType);

#ifdef IQWAMP_DEBUG_MODE
    qDebug() << "Reserve message" << message;
#endif

    QJsonParseError error;
    QJsonDocument messageDoc = QJsonDocument::fromJson(message.toLocal8Bit(), &error);

    if (error.error) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << "Message is not formatted correctly! Error: " << error.errorString();
#endif
        return false;
    }

    if (!messageDoc.isArray()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << "Message is not formatted correctly! Message must be JSON array.";
#endif
        return false;
    }

    *jsonMessage = messageDoc.array();
    if (jsonMessage->size() < 2) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << "Message is not formatted correctly! Message must be JSON array with size >= 2.";
#endif
        return false;
    }

    QJsonValue messageFirstParam = jsonMessage->first();
    if (!messageFirstParam.isDouble()) {
#ifdef IQWAMP_DEBUG_MODE
        qWarning() << "Message is not formatted correctly! Message must be JSON array with first int value.";
#endif
        return false;
    }

    *messageType = static_cast<IqWamp::MessageTypes>(messageFirstParam.toInt());

    return true;
}

QString IqWampJsonWebSocketHelper::messageTypeName(IqWamp::MessageTypes messageType)
{
    switch (messageType) {
        case IqWamp::MessageTypes::Welcome: return QStringLiteral("WELCOME");
        case IqWamp::MessageTypes::Subscribe: return QStringLiteral("SUBSCRIBE");
        case IqWamp::MessageTypes::Subscribed: return QStringLiteral("SUBSCRIBED");
        case IqWamp::MessageTypes::UnSubscribe: return QStringLiteral("UNSUBSCRIBE");
        case IqWamp::MessageTypes::Publish: return QStringLiteral("PUBLISH");
        case IqWamp::MessageTypes::Event: return QStringLiteral("EVENT");
        case IqWamp::MessageTypes::Register: return QStringLiteral("REGISTER");
        case IqWamp::MessageTypes::Registered: return QStringLiteral("REGISTERED");
        case IqWamp::MessageTypes::UnRegister: return QStringLiteral("UNREGISTER");
        case IqWamp::MessageTypes::Invocation: return QStringLiteral("INVOCATION");
        case IqWamp::MessageTypes::Call: return QStringLiteral("CALL");
        case IqWamp::MessageTypes::Result: return QStringLiteral("RESULT");
        default: return QStringLiteral("unknown_message_type");
    }

    return "unknown_message_type";
}
