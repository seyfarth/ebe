#include "ptyreader.h"
#include <unistd.h>
#include <stdio.h>


PtyReader::PtyReader(int fd, QString name, QWidget *parent)
: QThread()
{
    pty = fd;
    ptyName = name;
}


void PtyReader::run()
{
    char data[256];
    int n;

    printf("ready\n");
    while ( 1 ) {
        n=read(pty,data,256);
        printf("data\n");
        write(1,data,n);
        emit dataReady(data,n);
    }
}
