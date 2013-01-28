#ifndef GDB_H
#define GDB_H

#include "types.h"
#include <QThread>
#include <QProcess>
#include <QStringList>
#include <QList>
#include "variable.h"
#include "datawindow.h"

class FileLine
{
public:
    QString file;
    int line;
    FileLine(QString f, int l);
    bool operator==(FileLine &x) const;
};

uint qHash(const FileLine &f);

class GDBThread : public QThread
{
public:
    GDBThread();
    void run();
};

class GDB : public QObject
{
    Q_OBJECT

public:
    GDB();
    void initGdb();
    bool running;
    QHash<FileLine,int> bpHash;
    int numFloats;
    bool NullEOF;

private:
    void send(QString cmd, QString options="");
    QStringList sendReceive(QString cmd, QString options="");
    QStringList globals;
    QSet<QString> runCommands;
    QSet<QString> regs;
    QSet<QString> simpleTypes;
    void getBackTrace();
    void getRegs();
    void getFpRegs();
    void getGlobals();
    void getLocals();
    void getClasses();
    void getVars(QStringList &vars, QStringList &names, QStringList &types,
                 QStringList &values, QList<QList<int> > &dims );
    void getArgs();
    bool hasAVX;
    bool testAVX();
    QStringList classResults;
    void setNormal();

public slots:
    void doRun(QString exe, QString options, QStringList files,
               QList<IntSet> breakpoints, QStringList globals);
    void doNext();
    void doStep();
    void doContinue();
    void doStop();
    void getData(QStringList request);
    void requestVar(DataMap *map, QString name, QString address, QString type,
                    QString format, int size, int first, int last);
    void setBreakpoint(QString,int);
    void deleteBreakpoint(QString,int);
    void receiveWorkingDir(QString);
    void setEOF();

signals:
    void nextInstruction(QString,int);
    void sendRegs(StringHash);
    void sendFpRegs(QStringList);
    void sendData(QString,QString); 
    void sendGlobals(QStringList,QStringList,QStringList); 
    void sendLocals(QStringList,QStringList,QStringList); 
    void sendParameters(QStringList,QStringList,QStringList); 
    void sendClasses(QHash<QString,ClassDefinition> classes);
    void sendVar(DataMap *map, QString name, QString value);
    void dataReady(QStringList);
    void sendBackTrace(QStringList);
    void resetData();
    void endFlash();
};

#endif
