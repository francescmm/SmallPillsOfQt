/****************************************************************************************
 ** Tester is an application to test the Server.
 ** Copyright (C) 2016  Francesc Martinez <es.linkedin.com/in/cescmm/en>
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 3 of the License, or (at your option) any later version.
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
#ifndef Tester_H
#define Tester_H

#include <QDialog>
#include <QTcpSocket>
#include <QTimer>
#include <QMutex>
#include "ui_Tester.h"

namespace Ui
{
    class Tester;
}

class Tester : public QDialog
{
    Q_OBJECT

    signals:
        void updateModel();

    public:
        Tester(const QString &nif = "");
        ~Tester();
        static QString user;

    public slots:
        void setUserLogged(const QString &nif) { user = nif; }

    private slots:
        void connected();
        void readMsg();
        void connectionClosedByServer();
        void restoreConnection();
        void sendMsg();
        void closeSocket();

    private:
        Ui::Tester *ui;
        QMutex mutex;
        static QString IP;
        static int PORT;
        QTcpSocket *socket;
        //GadaSocket *socket;
};

#endif
