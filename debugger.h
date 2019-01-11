#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "types.h"
#include <QThread>
#include <QProcess>
#include <QStringList>
#include <QList>
#include <QSemaphore>
#include "variable.h"
#include "datawindow.h"

void acquireSem(QSemaphore &sem);
void releaseSem(QSemaphore &sem);

class Debugger: public QObject
{
    Q_OBJECT

public:

    static QProcess *dbgProcess;
    static ClassDefinition latestClass;
    static volatile bool running;
    static IntHash sizeForType;
    static QSet<QString> simpleTypes;

#if defined(Q_OS_WIN)
    static bool needToKill;
    static bool needToWake;
    static HANDLE hProcess;
    static HANDLE hThread;
#else
    static bool dbgWaiting;
#endif

    static QProcess::ProcessState runningStatus;
    static char letterForSize[];
    static unsigned int reg_masks[];
    static QString reg_names[];

    static QHash<FileLabel, int> bpHash;
    static int numFloats;
    static bool NullEOF;
    static QString asmFile;
    static int asmLine;

    static QStringList globals;
    static QSet<QString> runCommands;
    static QSet<QString> regs;
    static QStringList fpRegs;

    Debugger();
    virtual void writeLine(QString cmd);
    virtual void initDBG()=0;
    virtual void getBackTrace()=0;
    virtual void getRegs()=0;
    virtual void getFpRegs()=0;
    virtual void getGlobals()=0;
    virtual void getClasses()=0;
    virtual void getVars(QStringList &names, VariableDefinitionMap &vars)=0;
    virtual void getArgs()=0;
    bool hasAVX;
    virtual bool testAVX()=0;
    QStringList classResults;
    virtual void setNormal()=0;

public slots:
    virtual void processTypedefRequest(QString,QString&)=0;
    virtual void processClassRequest(QString,ClassDefinition&)=0;
    virtual void doRun(QString exe, QString options, QStringList files,
        QList<StringSet> breakpoints, QStringList globals)=0;
    virtual void doNext()=0;
    virtual void doNextInstruction()=0;
    virtual void doStepInstruction()=0;
    virtual void doCall()=0;
    virtual void doStep()=0;
    virtual void doContinue()=0;
    virtual void doStop()=0;
    virtual void doCommand(QString cmd, QString tag="")=0;
    virtual void getData(QStringList request)=0;
    virtual void requestVar(DataPlank *p, QString name, QString address, QString type,
        QString format, int size, int frame )=0;
    virtual void setBreakpoint(QString, QString)=0;
    virtual void setBreakpointInternal(QString, QString)=0;
    virtual void deleteBreakpoint(QString, QString)=0;
    virtual void receiveWorkingDir(QString)=0;
    virtual void setEOF()=0;
    virtual void requestStack(int n)=0;
    virtual void requestReset()=0;
    virtual void requestAsmVariable(int i, uLong address, int n)=0;
    virtual void requestParameters(DataPlank *p, int frame)=0;
    virtual void requestLocals(DataPlank *p, int frame)=0;
    virtual void completeStep()=0;

signals:
    void sendAsmVariable(int i, QStringList results);
    void receiveStack(QStringList results);
    void nextInstruction(QString,int);
    void sendRegs(StringHash);
    void sendFpRegs(QStringList);
    void sendData(QString, QString);
    void sendGlobals(VariableDefinitionMap);
    void sendLocals(DataPlank *p, VariableDefinitionMap);
    void sendParameters(DataPlank *p, VariableDefinitionMap);
    void sendClasses(QHash<QString, ClassDefinition> classes);
    void sendVar(DataPlank *p, QString name, QStringList values);
    void dataReady(QStringList);
    void sendBackTrace(QStringList);
    void resetData();
    void endFlash();
    void log(QString);
    void error(QString);
};

#endif
