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
#ifndef Server_H
#define Server_H

#include <QStringList>
#include <QTcpServer>
#include <QThread>
#include "ClientThread.h"
#include "Protocol.h"

class Server : public QTcpServer
{
    Q_OBJECT

    public:
        Server(QObject *parent = 0);
        ~Server();

        Client * getClient(int _id) const;
        Client * getClientByName(const QString &_name) const;
        QList<Client*> getClients() const;
      void writeToClients(const QString &_msg);

    protected:
        void incomingConnection(qintptr socketDescriptor);

    private:
        QStringList fortunes;
        QVector<ClientThread*> clientList;
        void clientToClientMsg(const Protocol &p);

    private slots:
        void processMsg(const QString &msg);
        void threadFinished(int threadId);
        void multicastMsg(Client *client, const Protocol &p);
};

#endif
