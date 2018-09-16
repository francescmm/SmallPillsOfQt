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
 
#ifndef Client_H
#define Client_H

#include <QTcpSocket>
#include <QTimer>
#include <QMap>
#include <QSet>
#include <QStringList>
#include <QHostAddress>

class Client : public QTcpSocket
{
    Q_OBJECT

    public:
        Client(int _id = -1, const QString &_name = "", QObject *parent = 0);
        Client & operator = (const Client &c);

        void setName(const QString &_name) { m_name = _name; }
        QString getName() const { return m_name; }
        void setId(int _id) { setSocketDescriptor(_id); }
        int getId() const { return socketDescriptor(); }
        QString getIpAddress() const { return peerAddress().toString(); }

    private:
        QString m_name;
};

#endif // Client_H
