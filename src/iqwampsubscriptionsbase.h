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

#ifndef IQWAMPSUBSCRIPTIONSBASE_H
#define IQWAMPSUBSCRIPTIONSBASE_H

#include <QObject>
#include <QHash>
#include <QSharedPointer>
#include "iqwampsubscription.h"

class IqWampSubscriptionsBase: public QObject
{
    Q_OBJECT
public:
    explicit IqWampSubscriptionsBase(QObject *parent = 0);

    QSharedPointer<IqWampSubscription> subscription(const QString &topic) const;
    QSharedPointer<IqWampSubscription> subscription(int id) const;
    QSharedPointer<IqWampSubscription> take(int id);

    bool hasSubscription(const QString &topic) const;
    bool hasSubscription(int id) const;

protected:
    bool addSubscription(QSharedPointer<IqWampSubscription> &&subscription);
    bool removeSubscription(QSharedPointer<IqWampSubscription> &subscription);

private:
    QHash<int, QSharedPointer<IqWampSubscription> > m_subscriptionOnId;
    QHash<QString, QSharedPointer<IqWampSubscription> > m_subscriptionOnTopic;
};

#endif //IQWAMPSUBSCRIPTIONSBASE_H
