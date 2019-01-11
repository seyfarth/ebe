#include "debugger.h"

#include <unistd.h>

QProcess *Debugger::dbgProcess;
ClassDefinition Debugger::latestClass;
volatile bool Debugger::running;
IntHash Debugger::sizeForType;
QSet<QString> Debugger::simpleTypes;

#if defined(Q_OS_WIN)
bool Debugger::needToKill;
bool Debugger::needToWake;
HANDLE Debugger::hProcess;
HANDLE Debugger::hThread;
#else
bool Debugger::dbgWaiting;
#endif

QHash<FileLabel, int> Debugger::bpHash;
int Debugger::numFloats;
bool Debugger::NullEOF;
QString Debugger::asmFile;
int Debugger::asmLine;
QStringList Debugger::globals;
QSet<QString> Debugger::runCommands;
QSet<QString> Debugger::regs;
QStringList Debugger::fpRegs;

QProcess::ProcessState Debugger::runningStatus=QProcess::NotRunning;
char Debugger::letterForSize[] = "bbhhwwwwg";

unsigned int Debugger::reg_masks[] = { 1, 4, 0x10, 0x40, 0x80, 0x400, 0x800 };
QString Debugger::reg_names[] = { "CF", "PF", "AF", "ZF", "SF", "DF", "OF" };

Debugger::Debugger()
    : QObject()
{
}

void Debugger::writeLine ( QString cmd )
{
    cmd += '\n';
    //qDebug() << "         writeLine" << cmd;
#if QT_VERSION >= 0x050000
    dbgProcess->write(cmd.toLocal8Bit().constData());
#else
    dbgProcess->write(cmd.toAscii());
#endif
}

void acquireSem ( QSemaphore &sem )
{
    int sleep=1;
    while ( !sem.tryAcquire(1) ) {
        QCoreApplication::processEvents(QEventLoop::AllEvents,sleep);
        //qDebug() << "waiting on semaphore";
        usleep(sleep);
        if ( sleep < 1000 ) sleep *= 2;
    }
}

void releaseSem ( QSemaphore &sem )
{
    //qDebug() << "releasing semaphore";
    sem.release(1);
    //QCoreApplication::processEvents(QEventLoop::AllEvents,10);
}

