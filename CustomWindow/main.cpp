#include <QApplication>
#include "CustomWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    CustomWindow *myWindow = new CustomWindow();
    myWindow->show();

    return app.exec();
}
