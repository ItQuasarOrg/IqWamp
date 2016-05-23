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

#include "iqwampsubscription.h"

IqWampSubscription::IqWampSubscription(int id, const QString &topic, IqWampCallee *callee):
    m_topic(topic),
    m_id(id)
{
    addCallee(callee);
}

QString IqWampSubscription::topic() const
{
    return m_topic;
}

int IqWampSubscription::id() const
{
    return m_id;
}

bool IqWampSubscription::hasCallee(const IqWampCallee *callee) const
{
    return m_callees.contains(const_cast<IqWampCallee *>(callee));
}

void IqWampSubscription::addCallee(IqWampCallee *callee)
{
    m_callees.insert(callee);
}

void IqWampSubscription::removeCallee(const IqWampCallee *callee)
{
    m_callees.remove(const_cast<IqWampCallee *>(callee));
}

QSet<IqWampCallee *> IqWampSubscription::callees() const
{
    return m_callees;
}



