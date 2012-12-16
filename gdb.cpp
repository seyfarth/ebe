#include <QDebug>
#include <QTimer>
#include "gdb.h"
#include "terminalwindow.h"

extern TerminalWindow *terminalWindow;

extern GDB *gdb;

GDBThread::GDBThread()
: QThread()
{
}

void GDBThread::run()
{
    qDebug() << "thread" << currentThreadId();
    gdb = new GDB(this);
    exec();
}

GDB::GDB(QObject *parent)
: QObject(parent)
{
    gdb = new QProcess(this);
    gdb->start("gdb");
    qDebug() << "gdb state" << gdb->state();
    runCommands << "run" << "step" << "next" << "stepi" << "nexti"
                << "continue";
    initGdb();
}

void GDB::send(QString cmd)
{
    QRegExp rx1("at ([^:]*):([0-9]*)$");
    QRegExp rx2("^([0-9]+).*$");
    cmd += '\n';
    qDebug() << cmd.toAscii();
    gdb->write(cmd.toAscii());
    cmd.chop(1);
    QString result;
    result = gdb->readLine();
    result.chop(1);
    qDebug() << "result:" << result;
    int count = 1;
    while ( result.left(5) != "(gdb)" ) {
        gdb->waitForReadyRead(10);
        result = gdb->readLine();
        count++;
        if ( result.length() > 0 ) {
            result.chop(1);
            qDebug() << "result:" << result;
        }
        if ( runCommands.contains(cmd) ) {
            if ( rx1.indexIn(result) >= 0 ) {
                qDebug() << rx1.cap(0) << rx1.cap(1) << rx1.cap(2);
                emit nextInstruction(rx1.cap(1),rx1.cap(2).toInt());
            }
            if ( rx2.indexIn(result) >= 0 ) {
                qDebug() << rx2.cap(0) << rx2.cap(1);
                emit nextInstruction("",rx2.cap(1).toInt());
            }
        }
    }
    qDebug() << "count" << count;
}

QStringList GDB::sendReceive(QString cmd)
{
}

void GDB::initGdb()
{
    gdb->setTextModeEnabled(true);
    gdb->setReadChannel(QProcess::StandardOutput);
    send("set prompt (gdb)\\n");
    send("tty " + terminalWindow->ptyName);
}

//public slots:
void GDB::doRun(QString exe, QStringList files, QList<IntSet> breakpoints)
{
    int i;
    int length = files.length();
    qDebug() << "length" << length;
    send("kill");
    send("file \""+exe+"\"");
    send("delete breakpoints");
    for ( i = 0; i < length; i++ ) {
        qDebug() << files[i] << breakpoints[i];
        foreach ( int bp, breakpoints[i] ) {
            send(QString("break %1:%2").arg(files[i]).arg(bp) );
        }
    }
    send("run");
}

void GDB::doNext()
{
    qDebug() << "gdb next";
    send("next");
}

void GDB::doStep()
{
    send("step");
}

void GDB::doContinue()
{
    send("continue");
}

void GDB::doStop()
{
    send("kill");
}

void GDB::getRegs()
{
}

void GDB::getFpRegs(QStringList names)
{
}

void GDB::getData(QString request)
{
}
