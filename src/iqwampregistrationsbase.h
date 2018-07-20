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

#ifndef IQWAMPREGISTRATIONSBASE_H
#define IQWAMPREGISTRATIONSBASE_H

#include <QObject>
#include <QHash>
#include "iqwampabstractcallee.h"
#include "iqwampregistration.h"

class IqWampRegistrationsBase : public QObject
{
    Q_OBJECT
public:
    explicit IqWampRegistrationsBase(QObject *parent = 0);

    bool hasRegistration(const QString &procedure) const;
    bool hasRegistration(int id) const;

    QSharedPointer<IqWampRegistration> registration(const QString &procedure) const;
    QSharedPointer<IqWampRegistration> registration(int id) const;

    void remove(int id);

    QSharedPointer<IqWampRegistration> take(int id);

protected:
    bool addRegistration(QSharedPointer<IqWampRegistration> &&registration);

    void removeRegistration(const QSharedPointer<IqWampRegistration> &registration);

private:
    QHash<QString, QSharedPointer<IqWampRegistration> > m_registrationsOnProcedure;
    QHash<int, QSharedPointer<IqWampRegistration> > m_registrationsOnId;
};


#endif //IQWAMPREGISTRATIONSBASE_H
