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
 
#ifndef CommandLineApp_H
#define CommandLineApp_H
 
#include <QObject>
#include "Server.h"
#include <iostream>

using namespace std;

class CommandLineApp : public QObject
{
    Q_OBJECT

    signals:
        void quitApp();

    public:
        CommandLineApp(QObject *parent = 0);
		~CommandLineApp();

	public slots:
        void exec();

    private:
        Server *server;
		QThread *rootServerThread;
};

#endif
