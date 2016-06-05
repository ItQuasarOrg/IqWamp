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

#ifndef IQWAMPCLIENT_H
#define IQWAMPCLIENT_H

#include "iqwampabstractclient.h"
#include <QScopedPointer>

class IqWampClientPrivate;

class IqWampClient: public IqWampAbstractClient
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IqWampClient)
public:
    explicit IqWampClient(QObject *parent = 0);

    bool open(const QString &url, const QString &realm);

signals:
    void opened();

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

private:
    IqWampClientPrivate * const d_ptr;
};


#endif //IQMETEOGUI_IQWAMPCLIENT_H
