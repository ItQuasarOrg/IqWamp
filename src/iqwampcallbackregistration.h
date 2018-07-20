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

#ifndef IQWAMPCALLBACKREGISTRATION_H
#define IQWAMPCALLBACKREGISTRATION_H

#include "iqwampregistration.h"
#include "iqwampcallerror.h"
#include <functional>
#include <QJsonArray>
#include <QJsonObject>

class IqWampCallbackRegistration: public IqWampRegistration
{
public:
    explicit IqWampCallbackRegistration(int id,
                                        const QString &procedure,
                                        std::function<void(const QJsonArray &, const QJsonObject &)> callback,
                                        std::function<void(const IqWampCallError &)> errorCallback);

    void execute(const QJsonArray &arguments, const QJsonObject &argumentsKw);

    void executeError(const IqWampCallError &error);

public:
    std::function<void(const QJsonArray &, const QJsonObject &)> callback() const;

    std::function<void(const IqWampCallError &)> errorCallback() const;

private:
    std::function<void(const QJsonArray &, const QJsonObject &)> m_callback;
    std::function<void(const IqWampCallError &)> m_errorCallback;
};

#endif //IQWAMPCALLBACKREGISTRATION_H
