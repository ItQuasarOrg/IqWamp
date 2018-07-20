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

#include "iqwampcallbacksubscriptions.h"

IqWampCallbackSubscriptions::IqWampCallbackSubscriptions(QObject *parent) :
    IqWampSubscriptions<IqWampCallbackSubscription>(parent)
{
}

QSharedPointer<IqWampCallbackSubscription> IqWampCallbackSubscriptions::create(int id,
                                                                               const QString &topic,
                                                                               std::function<void(const QJsonArray &, const QJsonObject &)> callback)
{
    QSharedPointer<IqWampCallbackSubscription> subscription (new IqWampCallbackSubscription(id, topic, callback));
    addSubscription(qSharedPointerCast<IqWampSubscription>(subscription));
    return subscription;
}
