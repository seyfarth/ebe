#include <QDebug>
#include <QTimer>
#include "gdb.h"
#include "terminalwindow.h"

extern TerminalWindow *terminalWindow;

extern GDB *gdb;
QProcess *gdbProcess;

QString readLine()
{
    QString result="";
    int n;
    do {
        gdbProcess->waitForReadyRead(10);
        result += gdbProcess->readLine();
        n = result.length();
        qDebug() << result;
    } while ( n == 0 || result.at(n-1) != '\n' );
    result.chop(1);
    return result;
}

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
    gdbProcess = new QProcess(this);
    gdbProcess->start("gdb");
    qDebug() << "gdb state" << gdbProcess->state();
    runCommands << "run" << "step" << "next" << "stepi" << "nexti"
                << "continue";
    regs << "rax" << "rbx" << "rcx" << "rdx"
         << "rdi" << "rsi" << "rbp" << "rsp"
         << "r8"  << "r9"  << "r10" << "r11"
         << "r12" << "r13" << "r14" << "r15"
         << "rip" << "eflags";
    initGdb();
}

void GDB::send(QString cmd, QString options)
{
    QRegExp rx1("at ([^:]*):([0-9]*)$");
    QRegExp rx2("^([0-9]+).*$");
    qDebug() << cmd.toAscii();
    cmd += '\n';
    gdbProcess->write(cmd.toAscii());
    cmd.chop(1);
    QString result;
    result = readLine();
    qDebug() << "result:" << result;
    int count = 1;
    while ( result.left(5) != "(gdb)" ) {
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
        result = readLine();
        count++;
        qDebug() << "result:" << result;
    }
}

QStringList GDB::sendReceive(QString cmd, QString options)
{
    QStringList list;
    cmd += '\n';
    qDebug() << cmd.toAscii();
    gdbProcess->write(cmd.toAscii());
    QString result;
    result = readLine();
    //qDebug() << "result:" << result;
    int count = 1;
    while ( result.left(5) != "(gdb)" ) {
        list.append(result);
        result = readLine();
        count++;
        //qDebug() << "result:" << result;
    }
    //qDebug() << list;
    return list;
}

void GDB::initGdb()
{
    gdbProcess->setTextModeEnabled(true);
    gdbProcess->setReadChannel(QProcess::StandardOutput);
    send("set prompt (gdb)\\n");
    send("tty " + terminalWindow->ptyName);
}

//public slots:
void GDB::doRun(QString exe, QString options, QStringList files,
          QList<IntSet> breakpoints)
{
    int i;
    int length = files.length();
    qDebug() << "length" << length;
    send("kill");
    send("file \""+exe+"\"");
    send("delete breakpoints");
    send("set args "+options);
    for ( i = 0; i < length; i++ ) {
        foreach ( int bp, breakpoints[i] ) {
            qDebug() << files[i] << bp;
            qDebug() << QString("break %1:%2").arg(files[i]).arg(bp);
            send(QString("break %1:%2").arg(files[i]).arg(bp) );
        }
    }
    send("run");
    hasAVX = testAVX();
    getRegs();
    getFpRegs();
}

void GDB::doNext()
{
    qDebug() << "gdb next";
    send("next");
    getRegs();
    getFpRegs();
}

void GDB::doStep()
{
    send("step");
    getRegs();
    getFpRegs();
}

void GDB::doContinue()
{
    send("continue");
    getRegs();
    getFpRegs();
}

void GDB::doStop()
{
    send("kill");
}

void GDB::getRegs()
{
    QStringList results;
    QStringList parts;
    QMap<QString,QString> map;
    int index1, index2;
    results = sendReceive("info registers");

    foreach ( QString result, results ) {
        parts = result.split(QRegExp("\\s+"));
        if ( regs.contains(parts[0]) ) {
            if ( parts[0] == "eflags" ) {
                index1 = result.indexOf('[');
                index2 = result.indexOf(']');
                map[parts[0]] = result.mid(index1+2,index2-index1-3);
            } else {
                map[parts[0]] = parts[1];
            }
        }
    }
    qDebug() << map;
    emit sendRegs(map);
}

void GDB::getFpRegs()
{
    QStringList results;
    QStringList parts;
    QStringList data;
    QString result;
    QRegExp rx1("(0x[0-9A-Fa-f]+).*(0x[0-9A-Fa-f]+).*"
                "(0x[0-9A-Fa-f]+).*(0x[0-9A-Fa-f]+)");
    QRegExp rx2("(0x[0-9A-Fa-f]+).*(0x[0-9A-Fa-f]+)");
    int index1, index2;
    for ( int i = 0; i < 16; i++ ) {
        if ( hasAVX ) {
            results = sendReceive(QString("print/x $ymm%1.v4_int64").arg(i));
            result = "";
            foreach ( QString res, results ) {
                //qDebug() << res;
                //qDebug() << "index" << rx.indexIn(res);
                if ( rx1.indexIn(res) >= 0 ) {
                    //qDebug() << "res" << rx.cap(1);
                    data.append(rx1.cap(1)+" "+rx1.cap(2)+" "
                               +rx1.cap(3)+" "+rx1.cap(4));
                }
            }
        } else {
            results = sendReceive(QString("print/x $xmm%1.v2_int64").arg(i));
            result = "";
            foreach ( QString res, results ) {
                //qDebug() << res;
                //qDebug() << "index" << rx.indexIn(res);
                if ( rx2.indexIn(res) >= 0 ) {
                    //qDebug() << "res" << rx.cap(1);
                    data.append(rx2.cap(1)+" "+rx2.cap(2)+" 0x0 0x0");
                }
            }
        }
    }
    //qDebug() << data;
    emit sendFpRegs(data);
}

bool GDB::testAVX()
{
    QStringList results;
    QStringList parts;
    results = sendReceive("print $ymm0.v4_int64[0]");
    foreach ( QString result, results ) {
        parts = result.split(QRegExp("\\s+"));
        if ( parts.length() == 3 && parts[1] == "=" ) return true;
    }
    return false;
}

void GDB::getData(QString request)
{
}
