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

#ifndef IQWAMP_H
#define IQWAMP_H

#include "iqwamp_global.h"
#include <QString>

class IQWAMPSHARED_EXPORT IqWamp
{
public:
    static const int ProtocolVersion = 1;

    enum class ErrorMessage {
        Type,
        Call,
        Request,
        Details,
        Error,
        Arguments,
        ArgumentsKw
    };

    enum class RegisterMessage {
        Type,
        Request,
        Options,
        Procedure
    };

    enum class UnRegisterMessage {
        Type,
        Request,
        Registration
    };

    enum class CallMessage {
        Type,
        Request,
        Options,
        Procedure,
        Arguments,
        ArgumentsKw
    };

    enum class ResultMessage {
        Type,
        Request,
        Details,
        Arguments,
        ArgumentsKw
    };

    enum class SubscribeMessage {
        Type,
        Request,
        Options,
        Topic
    };

    enum class UnSubscribeMessage {
        Type,
        Request,
        Subscription
    };

    enum class SubscribedMessage {
        Type,
        Request,
        Subscription
    };

    enum class PublishMessage {
        Type,
        Request,
        Options,
        Topic,
        Arguments,
        ArgumentsKw
    };

    enum class EventMessage {
        Type,
        Subscription,
        Publication,
        Details,
        Arguments,
        ArgumentsKw
    };

    enum class MessageTypes {
        Hello = 1,
        Welcome = 2,
        Abort = 3,
        GoodBye = 4,
        Error = 8,
        Publish = 16,
        Published = 17,
        Subscribe = 32,
        Subscribed = 33,
        UnSubscribe = 34,
        UnSubscribed = 35,
        Event = 36,
        Call = 48,
        Result = 50,
        Register = 64,
        Registered = 65,
        UnRegister = 66,
        UnRegistered = 67,
        Invocation = 68,
        Yield = 70
    };

    struct Errors {
        static const QString NoSuchRealm;
        static const QString NoSuchSubscription;
        static const QString ProcedureAlreadyExists;
        static const QString NoSuchRegistration;
        static const QString NoSuchProcedure;
        static const QString Timeout;
        static const QString NotOwner;
        static const QString NotSubscribed;
        static const QString NotFoundTopic;
    };
};


#endif //IQWAMP_H
