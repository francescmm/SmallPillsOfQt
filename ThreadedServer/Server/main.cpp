#include <QCoreApplication>
#include <QFile>
#include <QAbstractEventDispatcher>
#include <QtGlobal>
#include <QDateTime>
#include <QTimer>
#include <QLogger.h>

#include "CommandLineApp.h"

using namespace QLogger;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QLoggerManager *manager = QLoggerManager::getInstance();
    qDebug()<<QDir::currentPath();
    QStringList moduleList;
    moduleList.append("Server");
    moduleList.append("ClientThread");
    manager->addDestination(QDir::currentPath() + "/logs/sever.log",QStringList("Server"), LogLevel::Trace);
    manager->addDestination(QDir::currentPath() + "/logs/client.log",QStringList("ClientThread"), LogLevel::Trace);

    CommandLineApp cmd;

   QObject::connect(&cmd, SIGNAL(quitApp()), &app, SLOT(quit()));
   QTimer::singleShot(500, &cmd, SLOT(exec()));

   return app.exec();
}

