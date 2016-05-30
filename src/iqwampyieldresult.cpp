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

#include "iqwampyieldresult.h"

IqWampYieldResult::IqWampYieldResult(const QJsonArray &arguments):
    m_arguments(arguments),
    m_id(0)
{
}

IqWampYieldResult::IqWampYieldResult(const QJsonObject &argumentsKw):
    m_argumentsKw(argumentsKw),
    m_id(0)
{
}

IqWampYieldResult::IqWampYieldResult(const QJsonArray &arguments, const QJsonObject &argumentsKw):
    m_arguments(arguments),
    m_argumentsKw(argumentsKw),
    m_id(0)
{
}

QJsonArray IqWampYieldResult::arguments() const
{
    return m_arguments;
}

void IqWampYieldResult::setArguments(const QJsonArray &arguments)
{
    m_arguments = arguments;
}

QJsonObject IqWampYieldResult::argumentsKw() const
{
    return m_argumentsKw;
}

void IqWampYieldResult::setArgumentsKw(const QJsonObject &argumentsKw)
{
    m_argumentsKw = argumentsKw;
}

int IqWampYieldResult::id() const
{
    return m_id;
}

void IqWampYieldResult::setId(int id)
{
    m_id = id;
}
