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

#ifndef IQWAMPCALLEEREGISTRATION_H
#define IQWAMPCALLEEREGISTRATION_H

#include "iqwampregistration.h"

class IqWampAbstractCallee;

class IqWampCalleeRegistration: public IqWampRegistration
{
public:
    explicit IqWampCalleeRegistration(int id, const QString &procedure, IqWampAbstractCallee *callee);

public:
    IqWampAbstractCallee *callee() const;

private:
    IqWampAbstractCallee *m_callee;
};

Q_DECLARE_METATYPE(QSharedPointer<IqWampCalleeRegistration>)

#endif //IQWAMPCALLEEREGISTRATION_H
