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

#ifndef IQWAMPABSTRACTCALLEE_H
#define IQWAMPABSTRACTCALLEE_H

#include <QJsonArray>
#include <QJsonObject>
#include <QSharedPointer>
#include "iqwampsubscription.h"
#include "iqwampregistration.h"
#include "iqwamp.h"

class IqWampAbstractCallee : public QObject
{
    Q_OBJECT
public:
    explicit IqWampAbstractCallee(QObject *parent = 0);

    virtual ~IqWampAbstractCallee();

    virtual void sendEvent(const QSharedPointer<IqWampSubscription> &subscription,
                           int publicationId,
                           const QJsonArray &arguments,
                           const QJsonObject &argumentsKw) = 0;

    virtual void sendInvocation(const QSharedPointer<IqWampRegistration> &registration,
                                int invocationId,
                                const QJsonArray &arguments,
                                const QJsonObject &argumentsKw) = 0;

    virtual void sendResult(int callId,
                            const QJsonObject &details,
                            const QJsonArray &arguments,
                            const QJsonObject &argumentsKw) = 0;

    virtual void sendCallError(int callId,
                               const QString &error,
                               const QJsonObject &details = QJsonObject()) = 0;
};


#endif //IQWAMPABSTRACTCALLEE_H
