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

#ifndef IQWAMPSUBSCRIPTIONS_H
#define IQWAMPSUBSCRIPTIONS_H

#include "iqwampsubscriptionsbase.h"
#include "iqwampabstractcallee.h"
#include "iqwampcalleesubscription.h"

template <typename T>
class IqWampSubscriptions : public IqWampSubscriptionsBase
{
public:
    explicit IqWampSubscriptions(QObject *parent = 0) :
        IqWampSubscriptionsBase(parent)
    {
    }

    QSharedPointer<T> subscription(const QString &topic) const
    {
        return qSharedPointerCast<T>(IqWampSubscriptionsBase::subscription(topic));
    }

    QSharedPointer<T> subscription(int id) const
    {
        return qSharedPointerCast<T>(IqWampSubscriptionsBase::subscription(id));
    }
};


#endif //IQWAMPSUBSCRIPTIONS_H
