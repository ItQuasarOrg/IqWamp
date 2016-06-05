/**********************************************************************************
 **
 ** Copyright © 2016 Pavel A. Puchkov 
 **
 ** This file is part of IqWamp.
 **
 ** IqWamp is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** IqWamp is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with IqWamp.  If not, see <http://www.gnu.org/licenses/>.
 **
 **********************************************************************************/

#ifndef IQWAMPLOCALCLIENT_H
#define IQWAMPLOCALCLIENT_H

#include "iqwampabstractclient.h"
#include "iqwamp_global.h"

class IqWampRealm;
class IqWampLocalCallee;

class IQWAMPSHARED_EXPORT IqWampLocalClient: public IqWampAbstractClient
{
    Q_OBJECT
public:
    explicit IqWampLocalClient(QObject *parent, IqWampRealm *realm);

protected:
    virtual bool subscribeToTopic(const QString &topic,
                                  const QJsonObject &options,
                                  std::function<void(const QJsonArray &, const QJsonObject &)> callback) Q_DECL_OVERRIDE;

    virtual bool publishEvent(const QString &topic,
                              const QJsonArray &arguments,
                              const QJsonObject &argumentsKw) Q_DECL_OVERRIDE;

    virtual bool registerProcedureCallback(const QString &procedure,
                                           std::function<IqWampYieldResult (const QJsonArray &, const QJsonObject &)> callback) Q_DECL_OVERRIDE;

    virtual bool callProcedure(const QString &procedure,
                               const QJsonArray &arguments,
                               const QJsonObject &argumentsKw,
                               std::function<void(const QJsonArray &, const QJsonObject &)> callback,
                               std::function<void(const IqWampCallError &error)> errorCallback) Q_DECL_OVERRIDE;

public:
    IqWampRealm *realm() const;

private:
    IqWampRealm *m_realm;
    IqWampLocalCallee *m_callee;
};

#endif //IQWAMPLOCALCLIENT_H
