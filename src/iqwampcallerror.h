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

#ifndef IQMETEOGUI_IQWAMPCALLERROR_H
#define IQMETEOGUI_IQWAMPCALLERROR_H

#include <QString>
#include <QJsonArray>
#include <QJsonObject>

class IqWampCallError
{
public:
    IqWampCallError (const QJsonObject &details, const QString &error, const QJsonArray &arguments, const QJsonObject &argumentsKw);

public:
    QString error() const;
    QJsonObject details() const;
    QJsonArray arguments() const;
    QJsonObject argumentsKw() const;

private:
    QJsonObject m_details;
    QString m_error;
    QString m_uri;
    QJsonObject m_details;
    QJsonArray m_arguments;
    QJsonObject m_argumentsKw;
};


#endif //IQMETEOGUI_IQWAMPCALLERROR_H
