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

#ifndef IQWAMPREGISTRATIONS_H
#define IQWAMPREGISTRATIONS_H

#include <QObject>
#include <QHash>
#include "iqwampabstractcallee.h"
#include "iqwampregistration.h"

class IqWampRegistrations : public QObject
{
    Q_OBJECT
public:
    explicit IqWampRegistrations(QObject *parent = 0);
    bool hasRegistration(const QString &procedure) const;
    bool hasRegistration(int id) const;
    bool hasRegistration(const IqWampAbstractCallee *callee) const;

    QSharedPointer<IqWampRegistration> registration(const QString &procedure) const;
    QSharedPointer<IqWampRegistration> registration(int id) const;
    QSharedPointer<IqWampRegistration> registration(const IqWampAbstractCallee *callee) const;

    QSharedPointer<IqWampRegistration> create(const QString &procedure, IqWampAbstractCallee *callee);

    void remove(int id);
    void remove(const IqWampAbstractCallee *callee);
private:
    int m_lastRegistrationId;
    QHash<QString, QSharedPointer<IqWampRegistration> > m_registrationsOnProcedure;
    QHash<int, QSharedPointer<IqWampRegistration> > m_registrationsOnId;
    QHash<IqWampAbstractCallee *, QSharedPointer<IqWampRegistration> > m_registrationsOnCallee;

    void removeRegistration(const QSharedPointer<IqWampRegistration> &registration);
};


#endif //IQMETEOSERVER_IQWAMPREGISTRATIONS_H
