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
 
#include "Client.h"

Client::Client(const int id, const QString &_name, QObject *parent) : QTcpSocket(parent)
{
    if (id != -1)
        setSocketDescriptor(id);
    if (_name != "")
        m_name = _name;
}

Client & Client::operator = (const Client &c)
{
    if (this != &c)
    {
        m_name = c.m_name;
        setSocketDescriptor(c.getId());
    }

    return *this;
}
