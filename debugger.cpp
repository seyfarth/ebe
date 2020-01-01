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

QHash<QSemaphore *, QString> semNames;
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

void acquireSem ( QSemaphore & /*sem*/ )
{
    //qDebug() << "acquireSem" << &sem << QThread::currentThread() << sem.available()
             //<< "   loop lvl" << QThread::currentThread()->loopLevel();
    //sem.acquire(1);
}

void releaseSem ( QSemaphore & /*sem*/ )
{
    //qDebug() << "releaseSem" << &sem << QThread::currentThread() << sem.available();
    //sem.release(1);
}
