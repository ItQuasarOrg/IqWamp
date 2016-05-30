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

#include "iqwampbroker.h"
#include "iqwampabstractcallee.h"

IqWampBroker::IqWampBroker(QObject *parent):
    QObject(parent),
    m_lastPublicationId(0)
{
}

int IqWampBroker::publish(const QSharedPointer<IqWampSubscription> &subscription,
                          const QJsonArray &arguments,
                          const QJsonObject &argumentsKw)
{
    int publicationId = m_lastPublicationId++;
    for(IqWampAbstractCallee *client: subscription->callees()) {
        client->sendEvent(subscription, publicationId, arguments, argumentsKw);
    }

    return publicationId;
}



