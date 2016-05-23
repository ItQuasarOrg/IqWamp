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

#include "iqwamprouter.h"
#include "iqwampcallee.h"
#include "iqwamprouter_p.h"

IqWampRouter::IqWampRouter(QObject *parent):
    QObject(parent),
    d_ptr(new IqWampRouterPrivate())
{
}

bool IqWampRouter::listen(quint16 port, QWebSocketServer::SslMode secureMode)
{
    Q_D(IqWampRouter);
    return d->listen(port, secureMode);
}

void IqWampRouter::addRealm(IqWampRealm *realm)
{
    Q_D(IqWampRouter);
    d->addRealm(realm);
}

