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

#include "iqwampregistrations.h"
#include <QSharedPointer>

IqWampRegistrations::IqWampRegistrations(QObject *parent):
    QObject(parent),
    m_lastRegistrationId(0)
{
}

bool IqWampRegistrations::hasRegistration(const QString &procedure) const
{
    return m_registrationsOnProcedure.contains(procedure);
}

bool IqWampRegistrations::hasRegistration(int id) const
{
    return m_registrationsOnId.contains(id);
}

bool IqWampRegistrations::hasRegistration(const IqWampAbstractCallee *callee) const
{
    return m_registrationsOnCallee.contains(const_cast<IqWampAbstractCallee *>(callee));
}

QSharedPointer<IqWampRegistration> IqWampRegistrations::registration(const QString &procedure) const
{
    return m_registrationsOnProcedure[procedure];
}

QSharedPointer<IqWampRegistration> IqWampRegistrations::registration(int id) const
{
    return m_registrationsOnId[id];
}

QSharedPointer<IqWampRegistration> IqWampRegistrations::registration(const IqWampAbstractCallee *callee) const
{
    return m_registrationsOnCallee[const_cast<IqWampAbstractCallee *>(callee)];
}

QSharedPointer<IqWampRegistration> IqWampRegistrations::create(const QString &procedure, IqWampAbstractCallee *callee)
{
    int id = m_lastRegistrationId++;
    QSharedPointer<IqWampRegistration> registration (new IqWampRegistration(id, procedure, callee));
    m_registrationsOnProcedure[procedure] = registration;
    m_registrationsOnId[id] = registration;
    m_registrationsOnCallee[callee] = registration;
    return registration;
}

void IqWampRegistrations::remove(int id)
{
    if (!hasRegistration(id))
        return;

    QSharedPointer<IqWampRegistration> registrationToRemove = registration(id);
    removeRegistration(registrationToRemove);
}

void IqWampRegistrations::remove(const IqWampAbstractCallee *callee)
{
    if (!hasRegistration(callee))
        return;

    QSharedPointer<IqWampRegistration> registrationToRemove = registration(callee);
    removeRegistration(registrationToRemove);
}

void IqWampRegistrations::removeRegistration(const QSharedPointer<IqWampRegistration> &registration)
{
    m_registrationsOnId.remove(registration->id());
    m_registrationsOnProcedure.remove(registration->procedure());
    m_registrationsOnCallee.remove(registration->callee());
}






