#include "ptyreader.h"
#ifdef Q_WS_WIN
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <QDebug>

#ifdef Q_WS_WIN
PtyReader::PtyReader(HANDLE h)
: QThread()
{
    pty = h;
}
#else
PtyReader::PtyReader(int fd)
    : QThread()
{
    pty = fd;
}
#endif

void PtyReader::run()
{
    char data[257];
#ifdef Q_WS_WIN
    DWORD n;
#else
    int n;
#endif

    //printf("ready\n");
    while (1) {
#ifdef Q_WS_WIN
        ReadFile ( pty, data, 256, &n, NULL );
#else
        n = read(pty, data, 256);
#endif
	    //qDebug() << "PtyReader n:" << n;
        if ( n < 1 ) {
#ifdef Q_WS_WIN
            Sleep(1);
#else
            usleep(500000);
#endif
            continue;
        }
        data[n] = 0;
        QString s(data);
        //qDebug() << "PtyReader data ready" << s;
        emit dataReady(s);
    }
}
