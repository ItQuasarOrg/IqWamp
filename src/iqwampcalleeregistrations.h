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

#ifndef IQWAMPCALLEEREGISTRATIONS_H
#define IQWAMPCALLEEREGISTRATIONS_H

#include "iqwampregistrations.h"
#include "iqwampcalleeregistration.h"

class IqWampCalleeRegistrations: public IqWampRegistrations<IqWampCalleeRegistration>
{
public:
    explicit IqWampCalleeRegistrations(QObject *parent = 0);

    using IqWampRegistrations<IqWampCalleeRegistration>::registration;
    QSharedPointer<IqWampCalleeRegistration> registration(const IqWampAbstractCallee *callee) const;

    QSharedPointer<IqWampCalleeRegistration> create(const QString &procedure, IqWampAbstractCallee *callee);

    using IqWampRegistrations<IqWampCalleeRegistration>::remove;
    void remove(const IqWampAbstractCallee *callee);

    using IqWampRegistrations<IqWampCalleeRegistration>::hasRegistration;
    bool hasRegistration(const IqWampAbstractCallee *callee) const;

private:
    int m_lastRegistrationId;
    QHash<IqWampAbstractCallee*, QSharedPointer<IqWampCalleeRegistration> > m_registrationsOnCallee;
};


#endif //IQWAMPCALLEEREGISTRATIONS_H
