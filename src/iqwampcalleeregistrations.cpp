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

#include "iqwampcalleeregistrations.h"

IqWampCalleeRegistrations::IqWampCalleeRegistrations(QObject *parent) :
    IqWampRegistrations<IqWampCalleeRegistration>(parent),
    m_lastRegistrationId(0)
{
}

QSharedPointer<IqWampCalleeRegistration> IqWampCalleeRegistrations::create(const QString &procedure, IqWampAbstractCallee *callee)
{
    int id = m_lastRegistrationId++;
    QSharedPointer<IqWampCalleeRegistration> registration (new IqWampCalleeRegistration(id, procedure, callee));
    m_registrationsOnCallee[callee] = registration;

    addRegistration(qSharedPointerCast<IqWampRegistration>(registration));

    return registration;
}

void IqWampCalleeRegistrations::remove(const IqWampAbstractCallee *callee)
{
    if (!hasRegistration(callee))
        return;

    QSharedPointer<IqWampCalleeRegistration> registrationToRemove = registration(callee);
    m_registrationsOnCallee.remove(registrationToRemove->callee());

    removeRegistration(qSharedPointerCast<IqWampRegistration>(registrationToRemove));
}

bool IqWampCalleeRegistrations::hasRegistration(const IqWampAbstractCallee *callee) const
{
    return m_registrationsOnCallee.contains(const_cast<IqWampAbstractCallee *>(callee));
}

QSharedPointer<IqWampCalleeRegistration> IqWampCalleeRegistrations::registration(const IqWampAbstractCallee *callee) const
{
    return m_registrationsOnCallee[const_cast<IqWampAbstractCallee *>(callee)];
}
