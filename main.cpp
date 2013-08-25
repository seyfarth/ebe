#include <QApplication>
#include <QResource>
#include <stdio.h>
#include <string.h>
#include <QTranslator>

#include "mainwindow.h"

bool userSetGeometry = false;
int  userWidth;
int  userHeight;
QTranslator translator;

MainWindow *mainWin;
int main(int argc, char *argv[])
{
#ifdef Q_OS_MAC
    setenv("QT_USE_NATIVE_WINDOWS","1",1);
#endif
    QApplication app(argc, argv);
    //QResource::registerResource(app.applicationDirPath()+"/ebe.rcc");

    if ( argc >= 3 ) {
        if ( strcmp(argv[1],"-g") == 0 ) {
            char *p = strchr(argv[2],'x');
            if ( p ) {
                userSetGeometry = true;
                userWidth = atoi(argv[2]);
                userHeight = atoi(p+1);
                argc -= 2;
                argv += 2;
            }
        }
    }
    mainWin = new MainWindow;
    mainWin->show();
    if ( argc > 1 ) mainWin->open(argv[1]);
    return app.exec();
}
