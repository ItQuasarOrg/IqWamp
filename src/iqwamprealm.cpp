/**********************************************************************************
 * Copyright © 2016 Pavel A. Puchkov                                              *
 *                                                                                *
 * This file is part of IqWamp                                                    *
 *                                                                                *
 * IqWamp is free software: you can redistribute it and/or modify                 *
 * it under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or              *
 * (at your option) any later version.                                            *
 *                                                                                *
 * IqWamp is distributed in the hope that it will be useful,                      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 * GNU Lesser General Public License for more details.                            *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public License       *
 * along with IqWamp.  If not, see <http://www.gnu.org/licenses/>.                *
 **********************************************************************************/

#include "iqwamprealm.h"
#include "iqwamprealm_p.h"

IqWampRealm::IqWampRealm(const QString &name, QObject *parent):
    QObject(parent),
    d_ptr(new IqWampRealmPrivate(this))
{
    Q_D(IqWampRealm);
    d->m_name = name;
}

QString IqWampRealm::name() const
{
    Q_D(const IqWampRealm);
    return d->m_name;
}

void IqWampRealm::addCallee(IqWampCallee *client)
{
    Q_D(IqWampRealm);
    d->addClient(client);
}

IqWampLocalClient * IqWampRealm::localClient() const
{
    Q_D(const IqWampRealm);
    return d->m_localClient;
}

IqWampRegistrations *IqWampRealm::registrations() const
{
    Q_D(const IqWampRealm);
    return d->registrations();
}

IqWampSubscriptions *IqWampRealm::subscriptions() const
{
    Q_D(const IqWampRealm);
    return d->subscriptions();
}

IqWampBroker *IqWampRealm::broker() const
{
    Q_D(const IqWampRealm);
    return d->broker();
}

IqWampDialer *IqWampRealm::dialer() const
{
    Q_D(const IqWampRealm);
    return d->dialer();
}



