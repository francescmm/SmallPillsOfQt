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

#include <QDateTime>
#include "ClientThread.h"
#include "version.h"

ClientThread::ClientThread(int _id, QObject *parent) : QThread(parent)
{
   socketId = _id;
   quit = false;
}

void ClientThread::run()
{
   client = new Client(socketId);

   connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
   connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);

   client->write("Welcome to the Server!");

   QLog_Trace("ClientThread", "New client from " + client->peerAddress().toString());

   while (!quit)
   {
      client->waitForReadyRead(1);
      for (int i = msgToWrite.size() - 1; i >= 0; i--)
      {
         client->write(msgToWrite.at(i).toLatin1());
         msgToWrite.removeAt(i);
      }
   }
}

void ClientThread::readyRead()
{
   emit receivedMessage(client->readAll());
}

void ClientThread::disconnected()
{
   emit clientClosed(socketId);

   client->deleteLater();

   quit = true;
}

void ClientThread::sendMessage(const QString &msg)
{
   QLog_Trace("ClientThread", "ClientThread::sendMessage()");
   msgToWrite.prepend(msg);
}
