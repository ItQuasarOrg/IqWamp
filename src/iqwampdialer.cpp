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

#include "iqwampdialer.h"

IqWampDialer::IqWampDialer(QObject *parent):
    QObject(parent),
    m_lastInvocationId(0)
{
}

int IqWampDialer::call(const QSharedPointer<IqWampRegistration> &registration,
                       const QJsonArray &arguments,
                       const QJsonObject &argumentsKw)
{
    int invocationId = m_lastInvocationId++;
    IqWampCallee *client = registration->callee();
    client->call(registration->id(), invocationId, arguments, argumentsKw);

    return invocationId;
}



