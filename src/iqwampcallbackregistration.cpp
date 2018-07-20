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

#include "iqwampcallbackregistration.h"

IqWampCallbackRegistration::IqWampCallbackRegistration(int id,
                                                       const QString &procedure,
                                                       std::function<void(const QJsonArray &,
                                                                          const QJsonObject &)> callback,
                                                       std::function<void(const IqWampCallError &)> errorCallback) :
    IqWampRegistration(id, procedure),
    m_callback(callback),
    m_errorCallback(errorCallback)
{
}

std::function<void(const QJsonArray &, const QJsonObject &)> IqWampCallbackRegistration::callback() const
{
    return m_callback;
}

std::function<void(const IqWampCallError &)> IqWampCallbackRegistration::errorCallback() const
{
    return m_errorCallback;
}

void IqWampCallbackRegistration::execute(const QJsonArray &arguments, const QJsonObject &argumentsKw)
{
    m_callback(arguments, argumentsKw);
}

void IqWampCallbackRegistration::executeError(const IqWampCallError &error)
{
    if (m_errorCallback)
        m_errorCallback(error);
}

