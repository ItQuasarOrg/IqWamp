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

#include "iqwampabstractclient.h"

IqWampAbstractClient::IqWampAbstractClient(QObject *parent) :
    QObject(parent)
{
}

IqWampAbstractClient::~IqWampAbstractClient()
{
}

bool IqWampAbstractClient::publish(const QString &topic, const QJsonObject &argumentsKw)
{
    return publishEvent(topic, QJsonArray(), argumentsKw);
}

bool IqWampAbstractClient::publish(const QString &topic, const QJsonArray &arguments)
{
    return publishEvent(topic, arguments, QJsonObject());
}

bool IqWampAbstractClient::publish(const QString &topic, const QJsonArray &arguments, const QJsonObject &argumentsKw)
{
    return publishEvent(topic, arguments, argumentsKw);
}

bool IqWampAbstractClient::registerProcedure(const QString &procedure,
                                             std::function<IqWampYieldResult(const QJsonArray &,
                                                                             const QJsonObject &)>&& callback)
{
    return registerProcedureCallback(procedure, callback);
}

bool IqWampAbstractClient::registerProcedure(const QString &procedure, std::function<IqWampYieldResult()>&& callback)
{
    std::function<IqWampYieldResult (const QJsonArray &, const QJsonObject &)> callbackFunction = std::bind(callback);

    return registerProcedureCallback(procedure, callbackFunction);
}

bool IqWampAbstractClient::registerProcedure(const QString &procedure, std::function<IqWampYieldResult(const QJsonArray &)>&& callback)
{
    std::function<IqWampYieldResult (const QJsonArray &, const QJsonObject &)> callbackFunction = std::bind(callback, std::placeholders::_1);

    return registerProcedureCallback(procedure, callbackFunction);
}

bool IqWampAbstractClient::registerProcedure(const QString &procedure, std::function<IqWampYieldResult(const QJsonObject &)>&& callback)
{
    std::function<IqWampYieldResult (const QJsonArray &, const QJsonObject &)> callbackFunction = std::bind(callback, std::placeholders::_2);

    return registerProcedureCallback(procedure, callbackFunction);
}