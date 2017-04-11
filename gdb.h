#ifndef GDB_H
#define GDB_H

#include "types.h"
#include <QThread>
#include <QProcess>
#include <QStringList>
#include <QList>
#include "variable.h"
#include "datawindow.h"

class GDBThread: public QThread
{
public:
    GDBThread();
    void run();
};

class GDB: public QObject
{
    Q_OBJECT

public:
    GDB();
    void initGdb();
    QHash<FileLabel, int> bpHash;
    int numFloats;
    bool NullEOF;
    QString asmFile;
    int asmLine;

private:
    void sync();
    void send(QString cmd, QString options = "");
    QStringList sendReceive(QString cmd, QString options = "");
    QStringList globals;
    QSet<QString> runCommands;
    QSet<QString> regs;
    void getBackTrace();
    void getRegs();
    void getFpRegs();
    void getGlobals();
    void getLocals();
    void getClasses();
    void getVars(QStringList &names, VariableDefinitionMap &vars);
    void getArgs();
    bool hasAVX;
    bool testAVX();
    QStringList classResults;
    void setNormal();

public slots:
    void processTypedefRequest(QString,QString&);
    void processClassRequest(QString,ClassDefinition&);
    void doRun(QString exe, QString options, QStringList files,
        QList<StringSet> breakpoints, QStringList globals);
    void doNext();
    void doNextInstruction();
    void doStepInstruction();
    void doCall();
    void doStep();
    void doContinue();
    void doStop();
    void doCommand(QString);
    void getData(QStringList request);
    void requestVar(DataPlank *p, QString name, QString address, QString type,
        QString format, int size, int frame );
    void setBreakpoint(QString, QString);
    void deleteBreakpoint(QString, QString);
    void receiveWorkingDir(QString);
    void setEOF();
    void requestStack(int n);
    void requestReset();
    void requestAsmVariable(int i, uLong address, int n);
    void requestParameters(DataPlank *p, int frame);
    void requestLocals(DataPlank *p, int frame);

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
