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

#ifndef IQWAMPJSONWEBSOCKET_H
#define IQWAMPJSONWEBSOCKET_H

#include "iqwamp.h"
#include <QWebSocket>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

class IqWampJsonWebSocketHelper
{
public:
    static void send(QWebSocket *socket, const QJsonArray &message);
    static void sendError(QWebSocket *socket, IqWamp::MessageTypes requestType, const QJsonValue &request, const QString &error, const QJsonObject &details = QJsonObject());
    static bool parseMessage(const QString &message, QJsonArray *jsonMessage, IqWamp::MessageTypes *messageType);
    static QString messageTypeName(IqWamp::MessageTypes messageType);
};


#endif //IQWAMPJSONWEBSOCKET_H
