#include <QApplication>
#include <QPalette>
#include <QColor>
#include <QFont>
#include <stdio.h>

#include "mainwindow.h"

QApplication *qapp;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qapp = &app;
    //QFont font = app.font();
    //int pointSize = font.pointSize();
    //printf("size: %d\n",pointSize);
    //font.setPixelSize ( 20 );
    //app.setFont(font);
    //QPalette pal = app.palette();
    //pal.setColor(QPalette::Window, QColor(238,238,248));
    //app.setPalette(pal);
    MainWindow mainWin;
    mainWin.resize(800,600);
    mainWin.show();
    return app.exec();
}
