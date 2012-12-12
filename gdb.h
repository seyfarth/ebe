#ifndef GDB_H
#define GDB_H

#include <QThread>
#include <QProcess>
#include <QString>
#include <QStringList>

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

public slots:
    void doRun(QString exe);
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
