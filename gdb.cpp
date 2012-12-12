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
    initGdb();
}

void GDB::send(QString cmd)
{
    cmd += '\n';
    qDebug() << "gdb state" << gdb->state();
    qDebug() << cmd.toAscii();
    gdb->write(cmd.toAscii());
    qDebug() << "gdb state" << gdb->state();
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
void GDB::doRun(QString exe)
{
    send("kill");
    send("file \""+exe+"\"");
    send("run");
}

void GDB::doNext()
{
}

void GDB::doStep()
{
}

void GDB::doContinue()
{
}

void GDB::doStop()
{
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
