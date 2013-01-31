#include <QApplication>
#include <QResource>
#include <stdio.h>

#include "mainwindow.h"

MainWindow *mainWin;
int main(int argc, char *argv[])
{
#ifdef Q_OS_MAC
    setenv("QT_USE_NATIVE_WINDOWS","1",1);
#endif
    QApplication app(argc, argv);
    QString home;
    home = getenv("EBE_HOME");
    if ( home == "" ) home = ".";
    QResource::registerResource(home+"/ebe.rcc");
    mainWin = new MainWindow;
    mainWin->show();
    if ( argc > 1 ) mainWin->open(argv[1]);
    return app.exec();
}
