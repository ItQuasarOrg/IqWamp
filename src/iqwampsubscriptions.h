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

#include <QObject>
#include "iqwampcallee.h"
#include "iqwampsubscription.h"

class IqWampSubscriptions : public QObject
{
    Q_OBJECT
public:
    explicit IqWampSubscriptions(QObject *parent = 0);

    bool hasSubscription(const QString &topic) const;
    bool hasSubscription(int id) const;

    QSharedPointer<IqWampSubscription> subscription(const QString &topic) const;
    QSharedPointer<IqWampSubscription> subscription(int id) const;

    QSharedPointer<IqWampSubscription> create(const QString &topic, IqWampCallee *callee);

    void remove(const IqWampCallee *callee);
private:
    int m_lastSubscriptionId;

    QHash<int, QSharedPointer<IqWampSubscription> > m_subscriptionOnId;
    QHash<QString, QSharedPointer<IqWampSubscription> > m_subscriptionOnTopic;
};


#endif //IQWAMPSUBSCRIPTIONS_H
