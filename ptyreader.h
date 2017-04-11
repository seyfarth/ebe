#ifndef PTYREADER_H
#define PTYREADER_H

#include <QThread>
#include <QString>
#include <QKeyEvent>
#ifdef Q_OS_WIN32
#include <windows.h>
#endif

class PtyReader: public QThread
{
    Q_OBJECT

public:
#ifdef Q_OS_WIN32
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
