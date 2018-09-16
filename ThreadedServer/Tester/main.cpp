#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include "Dialog.h"
#include "Tester.h"

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);

   Dialog d;

   d.exec();

   if (d.getUser() != "")
   {
      Tester *tester = new Tester(d.getUser());
      d.close();
      tester->show();
      return app.exec();
   }

   return 0;
}
