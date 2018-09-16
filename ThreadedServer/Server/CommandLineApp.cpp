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
 
#include <QCoreApplication>
#include <QtGui>
#include <QtNetwork>

#include "Client.h"
#include "CommandLineApp.h"
#include "version.h"

CommandLineApp::CommandLineApp(QObject *parent) : QObject(parent)
{
	server = new Server();
	server->listen(QHostAddress::LocalHost,5555);
	rootServerThread = new QThread();
	connect(rootServerThread, SIGNAL(finished()), rootServerThread, SLOT(deleteLater()));
	server->moveToThread(rootServerThread);
}
CommandLineApp::~CommandLineApp()
{
	rootServerThread->exit(0);
	rootServerThread->deleteLater();
	QLoggerManager *manager = QLoggerManager::getInstance();
	manager->closeLogger();
}

void CommandLineApp::exec()
{
    rootServerThread->start();

    QString option;
    QTextStream cout(stdout);
    QTextStream cin(stdin);

    for (int i = 0; i < 25; i++)
        cout<<endl;
    do
    {
        cout<<"# ";
        cout.flush();
        option = cin.readLine();

        if (option == "help")
        {
            cout<<endl<<endl;
            cout<<"Options:"<<endl<<endl;
            cout<<"v\tShows current server version."<<endl<<endl;
            cout<<"i id \n\tShows the client information of the specified client id."<<endl<<endl;
            cout<<"in name\n\tShows the client information of the specified client name."<<endl<<endl;
            cout<<"sa \tShow all clients."<<endl<<endl;
        }
        if (option == "v")
        {
            cout<<endl;
            cout<<QString("Server version %1\n").arg(VERSION_VS)<<endl;
            cout.flush();
        }
        else if (option == "sa")
        {
            cout<<endl;
            QList<Client*> clientList = server->getClients();

            if (clientList.size() > 0)
            {
                Client *client;

                cout<<"*-----------------------------------------------------*"<<endl;
                cout<<"|   CLIENT ID   |             CLIENT NAME             |"<<endl;
                cout<<"|-----------------------------------------------------|"<<endl;

                for (int i = 0; i < clientList.size(); i++)
                {
                    client = clientList.at(i);
                    if (client)
                    {
                        QString clientId = QVariant(client->getId()).toString();
                        QString clientName = client->getName();

                        int clIdSize = clientId.size();
                        for (int i = 0; i < 13 - clIdSize; i++)
                            clientId += " ";

                        int clNmSize = clientName.size();
                        for (int i = 0; i < 35 - clNmSize; i++)
                            clientName += " ";

                        cout<<QString("| %1 | %2 |").arg(clientId).arg(clientName)<<endl;
                    }
                }

                cout<<"*-----------------------------------------------------*"<<endl;
            }
            else
                cout<<"There are no clients connected."<<endl;
            cout<<endl;
        }
        else if (option.split(" ").at(0) == "i" or option.split(" ").at(0) == "in")
        {
            Client *client;

            if (option.split(" ").at(0) == "i")
            {
                int id = option.split(" ").at(1).toInt();
                client = server->getClient(id);
            }
            else
            {
                QString name = option.split(" ").at(1);
                client = server->getClientByName(name);
            }

            cout<<endl;

            if (client)
            {
                //First line
                cout<<"*-----------------------------------------------------*"<<endl;
                cout<<"|                     CLIENT INFO                     |"<<endl;
                cout<<"|-----------------------------------------------------|"<<endl;

                cout<<QString("| CLIENT ID     | %1 ").arg(client->getId());
                for (int i = 0; i < 35 - QVariant(client->getId()).toString().size(); i++)
                    cout<<" ";
                cout<<"|"<<endl;
                cout<<"|               |                                     |"<<endl;
                cout<<QString("| CLIENT NAME   | %1 ").arg(client->getName());
                for (int i = 0; i < 35 - client->getName().size(); i++)
                    cout<<" ";
                cout<<"|"<<endl;
                cout<<"|               |                                     |"<<endl;
                cout<<QString("| CLIENT IP     | %1 ").arg(client->getIpAddress());
                for (int i = 0; i < 35 - client->getIpAddress().size(); i++)
                    cout<<" ";
                cout<<"|"<<endl;
                cout<<"*-----------------------------------------------------*"<<endl;
            }
            else
                cout<<"There are no clients with this name."<<endl;

            cout<<endl;
        }
		else if (option.toInt() == 4)
			server->writeToClients("Test message...");
    }
    while (option != "quit" and option != "q");

    cout<<endl;

    server->writeToClients("Server disconnected!");

    emit quitApp();
}
