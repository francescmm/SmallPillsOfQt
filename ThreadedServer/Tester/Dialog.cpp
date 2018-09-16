/****************************************************************************************
 ** Tester is an application to test the Server.
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
#include "Dialog.h"
#include "ui_Dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(this, SIGNAL(accepted()), this, SLOT(slotUserLogged()));
    connect(this, SIGNAL(rejected()), this, SLOT(close()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::slotUserLogged()
{
    nif = ui->nif->text();
}
