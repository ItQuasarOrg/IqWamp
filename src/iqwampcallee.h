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

#ifndef IQWAMPSERVERCLIENT_H
#define IQWAMPSERVERCLIENT_H

#include <QWebSocket>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QHash>
#include "iqwamp.h"
#include "iqwampabstractcallee.h"

class IqWampRealm;
class IqWampCalleePrivate;

class IqWampCallee: public IqWampAbstractCallee
{
    Q_OBJECT
public:
    explicit IqWampCallee(QWebSocket *socket, QObject *parent = 0);

    virtual ~IqWampCallee();

    void sendWelcome();
    void sendAbort(const QString &reason, const QJsonObject &details = QJsonObject());

    void closeConnection();

    virtual void sendEvent(const QSharedPointer<IqWampSubscription> &subscription,
                           int publicationId,
                           const QJsonArray &arguments,
                           const QJsonObject &argumentsKw) Q_DECL_OVERRIDE;

    virtual void sendInvocation(const QSharedPointer<IqWampRegistration> &registration,
                                int invocationId,
                                const QJsonArray &arguments,
                                const QJsonObject &argumentsKw) Q_DECL_OVERRIDE;

    virtual void sendResult(int callId,
                            const QJsonObject &details,
                            const QJsonArray &arguments,
                            const QJsonObject &argumentsKw) Q_DECL_OVERRIDE;

    void sendCallError(int callId,
                       const QString &error,
                       const QJsonObject &details = QJsonObject()) Q_DECL_OVERRIDE;

signals:
    void disconnected();
    void hello(const QString &realm);

public:
    QString sessionId() const;

    void setRealm(IqWampRealm *realm);

private:
    void processTextMessage(const QString &message);

    static QString messageTypeName(IqWamp::MessageTypes messageType);

    void createSessionId();

    void send(const QJsonArray &message);
    void sendError(IqWamp::MessageTypes requestType, const QJsonValue &request, const QString &error, const QJsonObject &details = QJsonObject());

    void processSubscribe(const QJsonArray &jsonMessage);
    void sendSubscribed(const QJsonValue &request, int subscriptionId);

    void processUnSubscribe(const QJsonArray &jsonMessage);
    void sendUnSubscribed(const QJsonValue &request);

    void processPublish(const QJsonArray &jsonMessage);
    void sendPublished(const QJsonValue &request, int publicationId);

    void processRegister(const QJsonArray &jsonMessage);
    void sendRegistered(const QJsonValue &request, int registrationId);

    void processUnRegister(const QJsonArray &jsonMessage);
    void sendUnRegistered(const QJsonValue &request);

    void processCall(const QJsonArray &jsonMessage);

private:
    QWebSocket *m_socket;
    QString m_sessionId;
    IqWampRealm *m_realm;

    QHash<int, QJsonValue> m_callRequests;
};

#endif //IQWAMPSERVERCLIENT_H
