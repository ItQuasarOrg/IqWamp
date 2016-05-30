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

#include "iqwampregistration.h"
#include "iqwampabstractcallee.h"

IqWampRegistration::IqWampRegistration(int id, const QString &procedure, IqWampAbstractCallee *callee):
    m_procedure(procedure),
    m_id(id),
    m_callee(callee)
{
}

QString IqWampRegistration::procedure() const
{
    return m_procedure;
}

int IqWampRegistration::id() const
{
    return m_id;
}

IqWampAbstractCallee * IqWampRegistration::callee() const
{
    return m_callee;
}

