#ifndef PTYREADER_H
#define PTYREADER_H

#include <QThread>
#include <QString>
#include <QKeyEvent>
#ifdef Q_WS_WIN
#include <windows.h>
#endif

class PtyReader: public QThread
{
    Q_OBJECT

public:
#ifdef Q_WS_WIN
    PtyReader(HANDLE h);
    HANDLE pty;
#else
    PtyReader(int fd);
    int pty;
#endif
    void run();

    signals:
    void dataReady(QString);

};

#endif
