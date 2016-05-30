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

#ifndef IQWAMPSUBSCRIPTION_H
#define IQWAMPSUBSCRIPTION_H

#include <QString>
#include <QSet>

class IqWampAbstractCallee;

class IqWampSubscription
{
public:
    IqWampSubscription(int id, const QString &topic, IqWampAbstractCallee *callee);

    QString topic() const;

    int id() const;

    bool hasCallee(const IqWampAbstractCallee *callee) const;
    void addCallee(IqWampAbstractCallee *callee);
    void removeCallee(const IqWampAbstractCallee *callee);
    QSet<IqWampAbstractCallee *> callees() const;

private:
    QString m_topic;
    int m_id;
    QSet<IqWampAbstractCallee *> m_callees;

};


#endif //IQMETEOSERVER_IQWAMPSUBSCRIPTION_H
