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

#ifndef IQWAMPYIELDRESULT_H
#define IQWAMPYIELDRESULT_H

#include <QJsonArray>
#include <QJsonObject>

class IqWampYieldResult
{
public:
    IqWampYieldResult(const QJsonArray &arguments);
    IqWampYieldResult(const QJsonObject &argumentsKw);
    IqWampYieldResult(const QJsonArray &arguments, const QJsonObject &argumentsKw);

    QJsonArray arguments() const;
    void setArguments(const QJsonArray &arguments);

    QJsonObject argumentsKw() const;
    void setArgumentsKw(const QJsonObject &argumentsKw);

    int id() const;
    void setId(int id);

private:
    QJsonArray m_arguments;
    QJsonObject m_argumentsKw;
    int m_id;
};


#endif //IQWAMPYIELDRESULT_H
