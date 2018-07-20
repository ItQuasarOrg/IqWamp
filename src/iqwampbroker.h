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

#ifndef IQWAMPBROKER_H
#define IQWAMPBROKER_H

#include <QObject>
#include <QSharedPointer>
#include "iqwampcalleesubscription.h"
#include <QJsonArray>
#include <QJsonObject>

class IqWampBroker : public QObject
{
    Q_OBJECT
public:
    explicit IqWampBroker(QObject *parent = 0);

    int publish(const QSharedPointer<IqWampCalleeSubscription> &subscription,
                const QJsonArray &arguments,
                const QJsonObject &argumentsKw);

private:
    int m_lastPublicationId;
};


#endif //IQWAMPBROKER_H
