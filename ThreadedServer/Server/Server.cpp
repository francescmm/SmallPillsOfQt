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

#include <QtCore>
#include "Server.h"
#include "Client.h"
#include "version.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
}

Server::~Server()
{
}

void Server::incomingConnection(qintptr socketDescriptor)
{
   ClientThread *client = new ClientThread(socketDescriptor);
   connect(client, SIGNAL(receivedMessage(QString)), this, SLOT(processMsg(QString)));
   connect(client, SIGNAL(clientClosed(int)), this, SLOT(threadFinished(int)), Qt::DirectConnection);
   connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));

   clientList.append(client);

   client->start();
}

void Server::processMsg(const QString &msg)
{
    ClientThread *clSender = qobject_cast<ClientThread*>(sender());

    QLog_Trace("Server", "Received missage from client [" + clSender->getClient()->peerAddress().toString() + "]");

    Protocol protocol(msg);

    if (protocol.getHeader() == Protocol::CLIENT_INFO)
    {
        Client *client = clSender->getClient();
        client->setName(protocol.getOtherCommands().value("name").toString());

        //Notify the clients that new one is connected and then the Server sends the client list to the new client
        Protocol newUser;
        newUser.setHeader(Protocol::NEW_CLIENT);
        newUser.setBody("New user has enter!");
        newUser.setOtherCommands("user", protocol.getOtherCommands().value("name").toString());

        Protocol clientsList;
        clientsList.setHeader(Protocol::CLIENTS_LIST);
        clientsList.setBody("List of connected clients");

        for (int i = 0; i < clientList.size(); i++)
        {
            if (clientList.at(i)->getClient()->getId() != client->getId())
            {
                clientList.at(i)->sendMessage(newUser.getFullMessage().toLatin1());
                clientsList.setOtherCommands("user"+QVariant(clientList.at(i)->getClient()->getId()).toString(),clientList.at(i)->getClient()->getName());
            }
        }

        clSender->sendMessage(clientsList.getFullMessage().toLatin1());
        //client->waitForBytesWritten();
    }
    else if (protocol.getHeader() == Protocol::MULTICAST)
        multicastMsg(clSender->getClient(), protocol);
    else if (protocol.getHeader() == Protocol::MESSAGE)
        clientToClientMsg(protocol);
}

void Server::clientToClientMsg(const Protocol &p)
{
    for (int i = 0; i < clientList.size(); i++)
    {
        if (clientList.at(i)->getClient()->getName() == p.getCommandValue("receiver") and p.getBody() != "")
        {
            clientList.at(i)->sendMessage(p.getFullMessage().toLatin1());
            //clientList.at(i)->waitForBytesWritten();
            break;
        }
    }
}

void Server::threadFinished(int threadId)
{
   QString userExited;

   for (int i = 0; i < clientList.size(); i++)
   {
      if (clientList.at(i)->getThreadId() == threadId)
      {
         userExited = clientList.at(i)->getClient()->getName();
         clientList.removeAt(i);
      }
   }

   //Message to the other clients
   Protocol *newUser = new Protocol();
   newUser->setHeader(Protocol::CLIENT_EXIT);
   newUser->setBody("User has left!");
   newUser->setOtherCommands("user", userExited);

   for (int i = 0; i < clientList.size(); i++)
      clientList.at(i)->sendMessage(newUser->getFullMessage().toLatin1());

   delete newUser;

   QLog_Trace("Server", "Thread finished");
}

void Server::multicastMsg(Client *client, const Protocol &p)
{
   if (client != NULL)
   {
      Protocol *newMsg = new Protocol();
      newMsg->setHeader(Protocol::MESSAGE);
      newMsg->setBody(p.getBody());
      newMsg->setOtherCommands("sender", client->getName());

      QLog_Trace("Server", "Server message: " + p.getBody());

      for (int i = 0; i < clientList.size(); i++)
      {
         if (clientList.at(i)->getClient()->getId() != client->getId())
            clientList.at(i)->sendMessage(newMsg->getFullMessage());
         else
         {
            Protocol *pAux = new Protocol();
            pAux->setHeader(Protocol::MESSAGE);
            pAux->setBody("Message sent!");
            pAux->setOtherCommands("sender","SERVER");
            clientList.at(i)->sendMessage(pAux->getFullMessage());
            delete pAux;
         }
      }
      delete newMsg;
   }
}

QList<Client*> Server::getClients() const
{
    QList<Client*> clients;

    for (int i = 0; i < clientList.size(); i++)
        clients.append(clientList.at(i)->getClient());

    return clients;
}

Client * Server::getClient(int _id) const
{
    for (const auto client : qAsConst(clientList))
        if (client and client->getClient()->getId() == _id)
            return client->getClient();

    return NULL;
}

Client * Server::getClientByName(const QString &_name) const
{
    for (const auto client : qAsConst(clientList))
        if (client and client->getClient()->getName() == _name)
                return client->getClient();

    return NULL;
}

void Server::writeToClients(const QString &_msg)
{
   for (const auto client : qAsConst(clientList))
      if (client)
         client->sendMessage(_msg.toLatin1());
}
