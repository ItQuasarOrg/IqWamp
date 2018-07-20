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

#ifndef IQWAMPCALLEESUBSCRIPTIONS_H
#define IQWAMPCALLEESUBSCRIPTIONS_H

#include "iqwampsubscriptions.h"
#include "iqwampcalleesubscription.h"

class IqWampCalleeSubscriptions: public IqWampSubscriptions<IqWampCalleeSubscription>
{
public:
    explicit IqWampCalleeSubscriptions(QObject *parent = 0);

    QSharedPointer<IqWampCalleeSubscription> create(const QString &topic, IqWampAbstractCallee *callee);

    void remove(const IqWampAbstractCallee *callee);

private:
    int m_lastSubscriptionId;
    QMultiHash<const IqWampAbstractCallee *, QSharedPointer<IqWampCalleeSubscription> > m_subscriptionOnCallee;
};


#endif //IQWAMPCALLEESUBSCRIPTIONS_H
