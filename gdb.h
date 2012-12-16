#ifndef GDB_H
#define GDB_H

#include <QThread>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QList>
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
    GDB(QObject *parent);
    void initGdb();

private:
    QProcess *gdb;
    void send(QString cmd);
    QStringList sendReceive(QString cmd);
    QSet<QString> runCommands;

public slots:
    void doRun(QString exe, QStringList files, QList<IntSet> breakpoints);
    void doNext();
    void doStep();
    void doContinue();
    void doStop();
    void getRegs();
    void getFpRegs(QStringList names);
    void getData(QString request);

signals:
    void nextInstruction(QString file, int line);
    void sendRegs(QStringList data);
    void sendFpRegs(QStringList data);
    void sendData(QString request, QString data); 
};

#endif
