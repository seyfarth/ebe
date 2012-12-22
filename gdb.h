#ifndef GDB_H
#define GDB_H

#include <QThread>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QSet>

typedef QSet<int> IntSet;

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

private:
    void send(QString cmd, QString options="");
    QStringList sendReceive(QString cmd, QString options="");
    QStringList globals;
    QSet<QString> runCommands;
    QSet<QString> regs;
    QSet<QString> simpleTypes;
    void getRegs();
    void getFpRegs();
    void getGlobals();
    void getLocals();
    void getArgs();
    bool hasAVX;
    bool testAVX();

public slots:
    void doRun(QString exe, QString options, QStringList files,
               QList<IntSet> breakpoints, QStringList globals);
    void doNext();
    void doStep();
    void doContinue();
    void doStop();
    void getData(QString request);

signals:
    void nextInstruction(QString,int);
    void sendRegs(QMap<QString,QString>);
    void sendFpRegs(QStringList);
    void sendData(QString,QString); 
    void sendGlobals(QStringList,QStringList,QStringList); 
    void sendLocals(QStringList,QStringList,QStringList); 
    void sendParameters(QStringList,QStringList,QStringList); 
};

#endif
