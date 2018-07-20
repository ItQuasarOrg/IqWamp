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

#include "iqwampcallerror.h"

IqWampCallError::IqWampCallError(const QJsonObject &details,
                                 const QString &error,
                                 const QJsonArray &arguments,
                                 const QJsonObject &argumentsKw):
    m_details(details),
    m_error(error),
    m_arguments(arguments),
    m_argumentsKw(argumentsKw)
{
}

QString IqWampCallError::error() const
{
    return m_error;
}

QJsonObject IqWampCallError::details() const
{
    return m_details;
}

QJsonArray IqWampCallError::arguments() const
{
    return m_arguments;
}

QJsonObject IqWampCallError::argumentsKw() const
{
    return m_argumentsKw;
}
