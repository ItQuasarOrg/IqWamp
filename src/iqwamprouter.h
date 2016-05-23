/**********************************************************************************
 * Copyright © 2016 Pavel A. Puchkov                                              *
 *                                                                                *
 * This file is part of IqWamp.                                                   *
 *                                                                                *
 * IqWamp is free software: you can redistribute it and/or modify                 *
 * it under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or              *
 * (at your option) any later version.                                            *
 *                                                                                *
 * IqWamp is distributed in the hope that it will be useful,                      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                  *
 * GNU Lesser General Public License for more details.                            *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public License       *
 * along with IqWamp.  If not, see <http://www.gnu.org/licenses/>.                *
 **********************************************************************************/

#ifndef IQWAMPSERVER_H
#define IQWAMPSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include "iqwamp_global.h"
#include "iqwamprealm.h"

class IqWampRouterPrivate;

class IQWAMPSHARED_EXPORT IqWampRouter: public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IqWampRouter)
public:
    explicit IqWampRouter(QObject *parent = 0);

    bool listen(quint16 port, QWebSocketServer::SslMode secureMode = QWebSocketServer::NonSecureMode);

    void addRealm(IqWampRealm *realm);

protected:
    const QScopedPointer<IqWampRouterPrivate> d_ptr;
};

#endif //IQWAMPSERVER_H
