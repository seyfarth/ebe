#include <QDebug>
#include <QTimer>
#include "gdb.h"

GDB::GDB()
{
    gdb = new QProcess(this);
    gdb->start("gdb");
    qDebug() << "gdb state" << gdb->state();
}

void GDB::run()
{
    initGdb();
    QTimer::singleShot(1000,this,SLOT(doRun()));
    exec();
}

void GDB::initGdb()
{
}

//public slots:
void GDB::doRun()
{
    qDebug() << "gdb state" << gdb->state();
}

void GDB::doNext()
{
}

void GDB::doStep()
{
}

void GDB::doContinue()
{
}

void GDB::doStop()
{
}

void GDB::getRegs()
{
}

void GDB::getFpRegs(QStringList names)
{
}

void GDB::getData(QString request)
{
}
