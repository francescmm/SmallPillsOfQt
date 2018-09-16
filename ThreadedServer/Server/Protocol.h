/****************************************************************************************
 ** Server is an application to manage several clients inside a thread.
 ** Copyright (C) 2016  Francesc Martinez <es.linkedin.com/in/cescmm/en>
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2.1 of the License, or (at your option) any later version.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************************/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtCore>
#include <QDebug>
#include <QVariant>
#include <QStringList>
#include <QMap>

class Protocol
{
    public:
        enum MessageType { CLIENT_INFO = 0, MESSAGE, MULTICAST, XML, NEW_CLIENT, CLIENT_EXIT, CLIENTS_LIST };

        Protocol();
        Protocol(const QString &fullMessage);
        Protocol(const Protocol &p);
        ~Protocol();
        Protocol & operator = (const Protocol &p);
        friend QDebug operator << (QDebug d, const Protocol &p);

        MessageType getHeader() const { return header; }
        void setHeader(const MessageType &type) { header = type; }
        QString getBody() const { return body; }
        void setBody(const QString &message) { body = message; }
        QMap<QString,QVariant> getOtherCommands() const { return otherCommands; }
        QVariant getCommandValue(const QString &key) const { return otherCommands.value(key.toLower()); }
        void setOtherCommands(const QString &name, const QString &value) { otherCommands.insert(name,value); }
        void setOtherCommands(const QMap<QString,QVariant> &commands) { otherCommands = commands; }
        void setFullMessage(const QString &fullMessage);
        QString getFullMessage() const;

    private:
        MessageType header;
        QString body;
        QMap<QString,QVariant> otherCommands;
};

#endif // PROTOCOL_H
