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

#ifndef IQWAMPLOCALCALLEE_H
#define IQWAMPLOCALCALLEE_H

#include "iqwampabstractcallee.h"
#include <functional>
#include <QObject>
#include <QHash>
#include "iqwampyieldresult.h"

class IqWampLocalClient;

class IqWampLocalCallee: public IqWampAbstractCallee
{
    Q_OBJECT
public:
    explicit IqWampLocalCallee(IqWampLocalClient *client, QObject *parent = 0);

    virtual void sendEvent(const QSharedPointer<IqWampCalleeSubscription> &subscription,
                           int publicationId,
                           const QJsonArray &arguments,
                           const QJsonObject &argumentsKw) Q_DECL_OVERRIDE;

    virtual void sendInvocation(const QSharedPointer<IqWampRegistration> &registration,
                                int invocationId,
                                const QJsonArray &arguments,
                                const QJsonObject &argumentsKw) Q_DECL_OVERRIDE;

    virtual void sendResult(int invocationId,
                            const QJsonObject &details,
                            const QJsonArray &arguments,
                            const QJsonObject &argumentsKw) Q_DECL_OVERRIDE;

    virtual void sendCallError(int callId, const QString &error, const QJsonObject &details) Q_DECL_OVERRIDE;

    bool subscribe(const QString &topic);
    bool registerProcedure(const QString &procedure, std::function<IqWampYieldResult (const QJsonArray &, const QJsonObject &)> callback);

signals:
    void event(const QString &topic, int publicationId, const QJsonArray &arguments, const QJsonObject &argumentsKw);
    void result(int callId, const QJsonObject &details, const QJsonArray &arguments, const QJsonObject &argumentsKw);
    void callError(int callId, const QString &error, const QJsonObject &details);

private:
    IqWampLocalClient *m_localClient;

    QHash<QString, std::function<IqWampYieldResult (const QJsonArray &, const QJsonObject &)> > m_callbacks;
};


#endif //IQWAMPLOCALCALLEE_H
