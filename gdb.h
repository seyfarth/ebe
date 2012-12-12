#ifndef GDB_H
#define GDB_H

#include <QThread>
#include <QProcess>
#include <QString>
#include <QStringList>

class GDB : public QThread
{
    Q_OBJECT

public:
    GDB();
    void run();
    void initGdb();

private:
    QProcess *gdb;

public slots:
    void doRun();
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
