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

#include <QObject>
#include "iqwamp_global.h"

class IqWampRealm;

class IQWAMPSHARED_EXPORT IqWampLocalClient: public QObject
{
    Q_OBJECT
public:
    explicit IqWampLocalClient(QObject *parent, IqWampRealm *realm);

    bool publish(const QString &topic, const QJsonObject &argumentsKw);
    bool publish(const QString &topic, const QJsonArray &arguments);
    bool publish(const QString &topic, const QJsonArray &arguments, const QJsonObject &argumentsKw);

private:
    IqWampRealm *m_realm;
};


#endif //IQWAMPLOCALCLIENT_H
