/**********************************************************************************
 **
 ** Copyright © 2016 Pavel A. Puchkov 
 **
 ** This file is part of IqWamp.
 **
 ** IqWamp is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** IqWamp is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with IqWamp.  If not, see <http://www.gnu.org/licenses/>.
 **
 **********************************************************************************/

#ifndef IQWAMPREALM_P_H
#define IQWAMPREALM_P_H

#include <QObject>
#include <QSet>
#include "iqwampcalleesubscriptions.h"
#include "iqwampcalleeregistrations.h"
#include "iqwamprealm.h"

class IqWampCallee;
class IqWampLocalClient;

class IqWampRealmPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(IqWampRealm)
public:
    IqWampRealmPrivate(IqWampRealm *parent = 0);

    void addClient(IqWampCallee *client);

    IqWampCalleeSubscriptions *subscriptions() const;
    IqWampCalleeRegistrations *registrations() const;

    IqWampBroker *broker() const;
    IqWampDialer *dialer() const;

private:
    void onClientDisconnected();

public:
    IqWampRealm *q_ptr;
    QString m_name;

    QSet<IqWampCallee *> m_clients;

    IqWampLocalClient *m_localClient;

    IqWampCalleeSubscriptions *m_subscriptions;
    IqWampCalleeRegistrations *m_registrations;

    IqWampBroker *m_broker;
    IqWampDialer *m_dialer;
};

#endif //IQWAMPREALM_P_H
