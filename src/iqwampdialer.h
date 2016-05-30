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

#ifndef IQWAMPDIALER_H
#define IQWAMPDIALER_H

#include <QObject>
#include <QTimer>
#include <QSharedPointer>
#include <QHash>
#include <QPointer>
#include "iqwampregistration.h"
#include "iqwampyieldresult.h"

class IqWampAbstractCallee;

class IqWampDialer : public QObject
{
    Q_OBJECT
public:
    explicit IqWampDialer(QObject *parent = 0);

    int call(const QSharedPointer<IqWampRegistration> &registration,
             const QJsonArray &arguments,
             const QJsonObject &argumentsKw,
             IqWampAbstractCallee *sender);

    void processYield(const QSharedPointer<IqWampRegistration> &registration,
                      const IqWampYieldResult &result);

private:
    int m_lastInvocationId;

    int m_callIdleInterval;

    class IqWampInvocationFuture
    {
    public:
        explicit IqWampInvocationFuture();
        explicit IqWampInvocationFuture(int invocationId, IqWampAbstractCallee *sender);

        int invocationId;
        QPointer<IqWampAbstractCallee> sender;
        QSharedPointer<QTimer> idleTimer;
    };
    QHash<int, IqWampInvocationFuture> m_invocationFutures;

    Q_INVOKABLE void sendInvocation(QPointer<IqWampAbstractCallee> callee,
                                    const QSharedPointer<IqWampRegistration> &registration,
                                    int invocationId,
                                    const QJsonArray &arguments,
                                    const QJsonObject &argumentsKw);
};

#endif //IQWAMPDIALER_H
