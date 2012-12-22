#include "ptyreader.h"
#include <unistd.h>
#include <stdio.h>
#include <QDebug>


PtyReader::PtyReader(int fd, QString name)
: QThread()
{
    pty = fd;
    ptyName = name;
}


void PtyReader::run()
{
    char data[257];
    int n;

    //printf("ready\n");
    while ( 1 ) {
        n=read(pty,data,256);
        data[n] = 0;
        QString s(data);
        //qDebug() << s;
        emit dataReady(s);
    }
}
