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

#include "iqwampregistrationsbase.h"

IqWampRegistrationsBase::IqWampRegistrationsBase(QObject *parent):
    QObject(parent)
{
}

bool IqWampRegistrationsBase::hasRegistration(const QString &procedure) const
{
    return m_registrationsOnProcedure.contains(procedure);
}

bool IqWampRegistrationsBase::hasRegistration(int id) const
{
    return m_registrationsOnId.contains(id);
}

QSharedPointer<IqWampRegistration> IqWampRegistrationsBase::registration(const QString &procedure) const
{
    return m_registrationsOnProcedure[procedure];
}

QSharedPointer<IqWampRegistration> IqWampRegistrationsBase::registration(int id) const
{
    return m_registrationsOnId[id];
}

void IqWampRegistrationsBase::remove(int id)
{
    if (!hasRegistration(id))
        return;

    QSharedPointer<IqWampRegistration> registrationToRemove = registration(id);
    removeRegistration(registrationToRemove);
}

void IqWampRegistrationsBase::removeRegistration(const QSharedPointer<IqWampRegistration> &registration)
{
    m_registrationsOnId.remove(registration->id());
    m_registrationsOnProcedure.remove(registration->procedure());
}

bool IqWampRegistrationsBase::addRegistration(QSharedPointer<IqWampRegistration> &&registration)
{
    m_registrationsOnId[registration->id()] = registration;
    m_registrationsOnProcedure[registration->procedure()] = registration;

    return true;
}

QSharedPointer<IqWampRegistration> IqWampRegistrationsBase::take(int id)
{
    QSharedPointer<IqWampRegistration> result = registration(id);
    remove(id);

    return result;
}



