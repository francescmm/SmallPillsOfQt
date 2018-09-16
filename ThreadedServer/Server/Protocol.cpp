/****************************************************************************************
 ** Server is an application to manage several clients inside a thread.
 ** Copyright (C) 2013  Francesc Martinez <es.linkedin.com/in/cescmm/en>
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

#include "Protocol.h"
#include "version.h"
 
Protocol::Protocol()
{
    header = MESSAGE;
}

Protocol::Protocol(const QString &fullMessage)
{
    setFullMessage(fullMessage);
}

Protocol::Protocol(const Protocol &p)
{
    header = p.header;
    body = p.body;
    otherCommands = p.otherCommands;
}

Protocol::~Protocol()
{
    header = MESSAGE;
    body = "";
    otherCommands.clear();
}

Protocol & Protocol::operator = (const Protocol &p)
{
    if (this != &p)
    {
        header = p.header;
        body = p.body;
        otherCommands = p.otherCommands;
    }

    return *this;
}

QDebug operator << (QDebug d, const Protocol &p)
{
    d << p.getFullMessage();

    return d;
}

void Protocol::setFullMessage(const QString &fullMessage)
{
    if (fullMessage.startsWith("$") and fullMessage.endsWith("$"))
    {
        QStringList msg = fullMessage.trimmed().split(";");

        int type = msg.at(0).right(msg.at(0).size()-1).toInt();
        header = MessageType(type);

        if (msg.size() == 2)
            body = msg.at(1).left(msg.at(1).size()-1);
        else
            body = msg.at(1);

        QStringList commandList = msg.at(2).left(msg.at(2).size()-1).split(",");
        QStringList command;

		for (int i = 0; i < commandList.size(); i+=1)
		{
			command = commandList.at(i).split(":");
			if (command.size() % 2 == 0)
				otherCommands.insert(command.at(0).toLower(),QVariant(command.at(1)));
			else
				break;
		}
    }
}

QString Protocol::getFullMessage() const
{
    QString msg;

    msg = "$" + QVariant(header).toString() + ";" + body + ";";

    QStringList keys = otherCommands.keys();
    QString command = "";

    foreach (QString key, keys)
    {
        command = key + ":" + otherCommands.value(key).toString();
        msg = msg + command + ",";
    }
    msg += "$";

    return msg;
}
