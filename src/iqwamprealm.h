/**********************************************************************************
 * Copyright © 2016 Pavel A. Puchkov                                              *
 *                                                                                *
 * This file is part of IqWamp.                                                   *
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

#ifndef IQWAMPREALM_H
#define IQWAMPREALM_H

#include <QObject>
#include "iqwamp_global.h"
#include "iqwampcalleesubscriptions.h"
#include "iqwampcalleeregistrations.h"
#include "iqwampbroker.h"
#include "iqwampdialer.h"
#include "iqwamplocalclient.h"

class IqWampCallee;
class IqWampRealmPrivate;

class IQWAMPSHARED_EXPORT IqWampRealm: public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IqWampRealm)
public:
    explicit IqWampRealm(const QString &name, QObject *parent = 0);

    void addCallee(IqWampCallee *callee);

public:
    QString name() const;

    IqWampLocalClient * localClient() const;

    IqWampCalleeRegistrations *registrations() const;
    IqWampCalleeSubscriptions *subscriptions() const;
    IqWampBroker *broker() const;
    IqWampDialer *dialer() const;

protected:
    IqWampRealmPrivate * const d_ptr;
};

#endif //IQWAMPREALM_H
