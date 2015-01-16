#include <QDebug>
#include <QTimer>
#include <cstdio>
#include "gdb.h"
#include "settings.h"
#include "terminalwindow.h"
#ifdef Q_WS_WIN
#include <windows.h>
#endif

extern TerminalWindow *terminalWindow;
extern int wordSize;
extern QMap<FileLine, unsigned long> fileLineToAddress;
extern QHash<unsigned long, FileLine> addressToFileLine;
extern QString breakFile;
extern int breakLine;
extern QString gdbName;

extern GDB *gdb;
QProcess *gdbProcess;

IntHash sizeForType;
QSet<QString> simpleTypes;
char letterForSize[] = "bbhhwwwwg";

#ifdef Q_WS_WIN
bool needToKill;
bool needToWake;
HANDLE hProcess;
HANDLE hThread;
#else
bool gdbWaiting;
#endif

bool isFortran;

unsigned int reg_masks[] = { 1, 4, 0x10, 0x40, 0x80, 0x400, 0x800 };
QString reg_names[] = { "CF", "PF", "AF", "ZF", "SF", "DF", "OF" };

QString readLine()
{
    QString result = "";
    int n;
#ifndef Q_WS_WIN
    gdbWaiting = true;
#endif
    do {
        while (gdbProcess->bytesAvailable() == 0) {
            gdbProcess->waitForReadyRead(0);
            //qDebug() << "waiting" << gdbProcess->bytesAvailable();
        }
        result += gdbProcess->readLine();
        n = result.length();
        //qDebug() << result;
    } while (n == 0 || result.at(n - 1) != '\n');
    result.chop(1);
#ifndef Q_WS_WIN
    gdbWaiting = false;
#endif
    return result;
}

GDBThread::GDBThread()
    : QThread()
{
}

void GDBThread::run()
{
    //qDebug() << "thread" << currentThreadId();
    gdb = new GDB;
    exec();
}

GDB::GDB()
    : QObject()
{
#ifdef Q_WS_WIN
    needToKill = false;
#endif
    gdbProcess = new QProcess(this);
    gdbProcess->setProcessChannelMode(QProcess::MergedChannels);
    gdbProcess->start(gdbName);

    wordSize = ebe["build/word_size"].toInt();
    //qDebug() << "gdb state" << gdbProcess->state();
    runCommands << "run" << "step" << "next" << "stepi" << "nexti"
        << "continue";
    simpleTypes << "char" << "signed char" << "unsigned char" << "short"
        << "signed short" << "unsigned short" << "short int"
        << "signed short int" << "unsigned short int" << "short signed int"
        << "short unsigned int" << "int" << "signed int" << "unsigned int"
        << "signed" << "unsigned" << "long" << "signed long" << "unsigned long"
        << "long unsigned" << "long signed" << "long unsigned int"
        << "long signed int" << "long int" << "signed long int"
        << "unsigned long int" << "long signed int" << "long unsigned int"
        << "long long" << "signed long long" << "unsigned long long" << "bool"
        << "float" << "double" << "long double" << "logical(kind=1)"
        << "logical(kind=2)" << "logical(kind=4)" << "logical(kind=8)"
        << "integer(kind=1)" << "integer(kind=2)" << "integer(kind=4)"
        << "integer(kind=8)" << "real(kind=4)" << "real(kind=8)";
    sizeForType["bool"] = sizeof(bool);
    sizeForType["char"] = sizeof(char);
    sizeForType["signed char"] = sizeof(signed char);
    sizeForType["unsigned char"] = sizeof(unsigned char);
    sizeForType["short"] = sizeof(short);
    sizeForType["signed short"] = sizeof(signed short);
    sizeForType["unsigned short"] = sizeof(unsigned short);
    sizeForType["short int"] = sizeof(short);
    sizeForType["signed short int"] = sizeof(signed short int);
    sizeForType["unsigned short int"] = sizeof(unsigned short int);
    sizeForType["short signed int"] = sizeof(short signed int);
    sizeForType["short unsigned int"] = sizeof(short unsigned int);
    sizeForType["int"] = sizeof(int);
    sizeForType["signed int"] = sizeof(signed int);
    sizeForType["unsigned int"] = sizeof(unsigned int);
    sizeForType["signed"] = sizeof(signed);
    sizeForType["unsigned"] = sizeof(unsigned);
    sizeForType["long"] = sizeof(long);
    sizeForType["signed long"] = sizeof(signed long);
    sizeForType["unsigned long"] = sizeof(unsigned long);
    sizeForType["long int"] = sizeof(long int);
    sizeForType["signed long int"] = sizeof(signed long int);
    sizeForType["unsigned long int"] = sizeof(unsigned long int);
    sizeForType["long signed int"] = sizeof(long signed int);
    sizeForType["long unsigned int"] = sizeof(long unsigned int);
    sizeForType["long long"] = sizeof(long long);
    sizeForType["signed long long"] = sizeof(signed long long);
    sizeForType["unsigned long long"] = sizeof(unsigned long long);
    sizeForType["float"] = sizeof(float);
    sizeForType["double"] = sizeof(double);
    sizeForType["long double"] = sizeof(long double);
    sizeForType["logical(kind=1)"] = 1;
    sizeForType["logical(kind=2)"] = 2;
    sizeForType["logical(kind=4)"] = 4;
    sizeForType["logical(kind=8)"] = 8;
    sizeForType["integer(kind=1)"] = 1;
    sizeForType["integer(kind=2)"] = 2;
    sizeForType["integer(kind=4)"] = 4;
    sizeForType["integer(kind=8)"] = 8;
    sizeForType["real(kind=4)"] = 4;
    sizeForType["real(kind=8)"] = 8;
    if (wordSize == 64) {
        numFloats = 16;
        regs << "rax" << "rbx" << "rcx" << "rdx" << "rdi" << "rsi" << "rbp"
            << "rsp" << "r8" << "r9" << "r10" << "r11" << "r12" << "r13"
            << "r14" << "r15" << "rip" << "eflags";
    } else {
        numFloats = 8;
        regs << "eax" << "ebx" << "ecx" << "edx" << "edi" << "esi" << "ebp"
            << "esp" << "eip" << "eflags";
    }
    initGdb();
    NullEOF = false;
}


void GDB::sync()
{
    QString cmd;
    if ( isFortran ) {
        cmd="print \'sync\'\n";
    } else {
        cmd="print \"sync\"\n";
    }
    //qDebug() << cmd;
#if QT_VERSION >= 0x050000
    gdbProcess->write(cmd.toLocal8Bit().constData());
#else
    gdbProcess->write(cmd.toAscii());
#endif
    //emit log(cmd);
    QString result;
    result = readLine();
    //qDebug() << result;
    //emit log(result);
    while ( result.indexOf("sync") < 0 ) {
        if ( result.indexOf("Invalid") >= 0 ) {
            isFortran = true;
            break;
        }
        result = readLine();
        //qDebug() << result;
        //emit log(result);
    }
    while ( result.left(5) != "(gdb)" ) {
        result = readLine();
        //qDebug() << result;
        //emit log(result);
    }
}



void GDB::send(QString cmd, QString /*options*/)
{
    QRegExp rx1("at ([^:]*):([0-9]*)$");
    QRegExp rx2("^([0-9]+).*$");
    //qDebug() << cmd;
#ifdef Q_WS_WIN
    if ( needToWake && cmd == "continue" ) {
        //qDebug() << "ResumeThread" << hThread;
        ResumeThread(hThread);
        needToWake = false;
    }
#endif
    sync();
    cmd += '\n';
#if QT_VERSION >= 0x050000
    gdbProcess->write(cmd.toLocal8Bit().constData());
#else
    gdbProcess->write(cmd.toAscii());
#endif
    emit log(cmd);
    cmd.chop(1);
    QString result;
    result = readLine();
    emit log(result);
    //qDebug() << "result:" << result;
    if ( result.indexOf("Inferior") >= 0 && result.indexOf("exited") >= 0 ) {
        running = false;
    }
    while (result.left(5) != "(gdb)" && result.left(6) != "Contin") {
        //qDebug() << "result" << result;
        //if ( runCommands.contains(cmd) ) {
        //if ( rx1.indexIn(result) >= 0 ) {
        //qDebug() << rx1.cap(0) << rx1.cap(1) << rx1.cap(2);
        //emit nextInstruction(rx1.cap(1),rx1.cap(2).toInt());
        //}
        //if ( rx2.indexIn(result) >= 0 ) {
        //qDebug() << rx2.cap(0) << rx2.cap(1);
        //emit nextInstruction("",rx2.cap(1).toInt());
        //}
        //}
        result = readLine();
        emit log(result);
        if ( result.indexOf("Inferior") >= 0 &&
             result.indexOf("exited") >= 0 ) {
            running = false;
        }
        if (result.startsWith("Program received signal")) emit error(result);
        //qDebug() << "result:" << result;
    }
}

QStringList GDB::sendReceive(QString cmd, QString /*options*/)
{
    QStringList list;
    //qDebug() << cmd;
    cmd += '\n';
    sync();
    //qDebug() << cmd;
#if QT_VERSION >= 0x050000
    gdbProcess->write(cmd.toLocal8Bit().constData());
#else
    gdbProcess->write(cmd.toAscii());
#endif
    emit log(cmd);
    QString result;
    result = readLine();
    emit log(result);
    if ( result.indexOf("Inferior") >= 0 && result.indexOf("exited") >= 0 ) {
        running = false;
    }
    //qDebug() << "result:" << result;
    while (result.right(5) != "(gdb)" && result.right(6) != "Contin") {
        list.append(result);
        result = readLine();
        emit log(result);
        if ( result.indexOf("Inferior") >= 0 &&
             result.indexOf("exited") >= 0 ) {
            running = false;
        }
        //qDebug() << "result:" << result;
    }
    if (result.length() > 5) list.append(result);
    //qDebug() << list;
    return list;
}

void GDB::getClasses()
{
    QHash < QString, ClassDefinition > classes;
    ClassDefinition c;
    VariableDefinition v;
    QString result;
    QStringList results;
    QString name;
    QStringList parts;
    QStringList names;
    QRegExp rx("^\\s+([a-zA-Z].*)\\s+(\\**)([a-zA-Z0-9][a-zA-Z0-9_]*)(.*);$");

    foreach(result, classResults)
    {
        parts = result.split(QRegExp("\\s+"));
        if (parts.length() == 1 && parts[0].length() > 0) {
            name = parts[0];
            if (name[name.length() - 1] == ';') {
                name.chop(1);
                names.append(name);
            }
        } else if (parts.length() == 2 && parts[0] == "struct") {
            name = parts[1];
            if (name[name.length() - 1] == ';') {
                name.chop(1);
                names.append(name);
            }
        }
    }
    foreach(name, names)
    {
        //qDebug() << "gc" << name;
        c.name = name;
        c.members.clear();
        results = sendReceive("ptype " + name);
        foreach(result, results)
        {
            if (result.indexOf("    ") >= 0 && result.indexOf('(') < 0) {
                //qDebug() << result;
                if (rx.indexIn(result) >= 0) {
                    //qDebug() << "match" << rx.cap(1) << rx.cap(2) << rx.cap(3) <<
                    //rx.cap(4);
                    v.name = rx.cap(3);
                    v.type = rx.cap(1) + " " + rx.cap(2) + rx.cap(4);
                    if (v.type[v.type.length() - 1] == ' ') v.type.chop(1);
                    v.isSimple = simpleTypes.contains(v.type);
                    //qDebug() << v.name << v.type << v.isSimple;
                    c.members.append(v);
                }
            }
        }
        classes[name] = c;
    }
    emit sendClasses(classes);
}

void GDB::initGdb()
{
    running = false;
    gdbProcess->setTextModeEnabled(true);
    gdbProcess->setReadChannel(QProcess::StandardOutput);
    send("set prompt (gdb)\\n");
}

//public slots:
void GDB::doCommand(QString cmd)
{
    send(cmd);
}

void GDB::doRun(QString exe, QString options, QStringList files,
    QList<StringSet> breakpoints, QStringList g)
{
    int i;
    int length = files.length();
    isFortran = false;
    globals = g;
    globals.append("stack");
    //qDebug() << "length" << length;
    running = false;
    bpHash.clear();
    send("kill");
    send("nosharedlibrary");
    send(QString("cd ") + QDir::currentPath());
    //qDebug() << "file" << exe;
    send("file \"" + exe + "\"");
    classResults = sendReceive("info types ^[[:alpha:]][[:alnum:]_]*$");
    send("delete breakpoints");
    for (i = 0; i < length; i++) {
        foreach ( QString bp, breakpoints[i] ) {
            //qDebug() << files[i] << bp;
            setBreakpoint(files[i],bp);
        }
    }

#ifdef Q_WS_WIN
    needToWake = true;
    //char dir[1025];
    //DWORD len = 1024;
    //len = GetCurrentDirectory(len,(LPTSTR)dir);
    //printf("len %d, dir = %s\n",len,dir);
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    DWORD flags;
    ZeroMemory(&si,sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi,sizeof(pi));
    si.hStdInput = terminalWindow->childStdin;
    si.hStdOutput = terminalWindow->childStdout;
    si.hStdError = terminalWindow->childStdout;
    flags = CREATE_SUSPENDED;
    si.dwFlags = STARTF_USESTDHANDLES;
    QString cmd = "\"" + exe + "\" " + options;
    QByteArray ba = cmd.toLocal8Bit();
    //qDebug() << "cmd" << cmd;
    const char *s = ba.data();
    size_t sz = ba.length()+1;
    wchar_t * ws = new wchar_t[sz];
    size_t conv=0;
    mbstowcs_s(&conv,ws,sz,s,_TRUNCATE);
    //printf("cmd = %s\n",s);
    if ( needToKill ) {
        TerminateProcess(hProcess,0);
        needToKill = false;
    }
    if (! CreateProcess (
            NULL,           // Application name
            ws,// Command line
            NULL,// Process attributes
            NULL,// Thread attributes
            TRUE,// Inherit handles
            flags,// Creation flags
            NULL,// Environment
            NULL,// Current directory
            &si,// Startup info
            &pi)// Process information
    ) {
        qDebug() << tr("Could not create process") << exe;
        DWORD error = GetLastError();
        qDebug() << tr("error") << error;
        return;
    }
    //qDebug() << "pid" << pi.dwProcessId;
    hProcess = pi.hProcess;
    hThread = pi.hThread;
    needToKill = true;

    send("attach "+QString("%1").arg(pi.dwProcessId));
    //send("delete breakpoints");
    //for ( i = 0; i < length; i++ ) {
    //foreach ( QString bp, breakpoints[i] ) {
    ////qDebug() << files[i] << bp;
    ////qDebug() << QString("break %1:%2").arg(files[i]).arg(bp);
    //send(QString("break %1:%2").arg(files[i]).arg(bp) );
    //}
    //}
    send("continue");
    //send("set prompt (gdb)\\n");
#else
    send("tty " + terminalWindow->ptyName);
    send("set args " + options);
    send("run");
#endif
    running = true;
    if (!running) return;
    hasAVX = testAVX();
    if (!running) return;
    setNormal();
    if (!running) return;
    getRegs();
    getBackTrace();
    if (!running) return;
    getFpRegs();
    getGlobals();
    //qDebug() << "run";
    //if ( running ) emit resetData();
    //qDebug() << "run";
    getClasses();
    //qDebug() << "Done run";
}

void GDB::doNext()
{
    //qDebug() << "gdb next";
    if (!running) return;
    send("next");
    setNormal();
    if (!running) return;
    getRegs();
    getBackTrace();
    if (!running) return;
    getFpRegs();
    getGlobals();
    //if ( running ) emit resetData();
}

void GDB::doStep()
{
    //qDebug() << "gdb step";
    if (!running) return;
    send("step");
    setNormal();
    if (!running) return;
    getRegs();
    getBackTrace();
    if (!running) return;
    getFpRegs();
    getGlobals();
    //if ( running ) emit resetData();
}

void GDB::doNextInstruction()
{
    //qDebug() << "nexti";
    if (!running) return;
    send("nexti");
    setNormal();
    if (!running) return;
    getRegs();
    getBackTrace();
    if (!running) return;
    getFpRegs();
    getGlobals();
    //if ( running ) emit resetData();
}

void GDB::doStepInstruction()
{
    //qDebug() << "stepi";
    if (!running) return;
    send("stepi");
    setNormal();
    if (!running) return;
    getRegs();
    getBackTrace();
    if (!running) return;
    getFpRegs();
    getGlobals();
    //if ( running ) emit resetData();
}

void GDB::doCall()
{
    FileLine fl(breakFile, breakLine + 1);
    //qDebug() << "call from" << breakFile << breakLine;
    //for ( int i = 1; i < 100; i++ ) {
        //FileLine fl2(breakFile, i );
        //qDebug() << "fl2" << i << fileLineToAddress[fl2];
    //}
    if (!running) return;
    if ( ebe["build/assembler"].toString() == "yasm" ) {
        //qDebug() << "tbreak *" << fileLineToAddress[fl] << "OK";
        send(QString("tbreak *%1").arg(fileLineToAddress[fl]));
//#if defined(Q_WS_WIN) || defined(Q_OS_MAC)
        //send(QString("tbreak *%1").arg(fileLineToAddress[fl]));
//#else
    } else {
        send(QString("tbreak \"%1\":%2").arg(breakFile).arg(breakLine+1));
    }
//#endif
    send("continue");
    setNormal();
    if (!running) return;
    getRegs();
    getBackTrace();
    if (!running) return;
    getFpRegs();
    getGlobals();
    //if ( running ) emit resetData();
}

void GDB::doContinue()
{
    //qDebug() << "continue";
    if (!running) return;
    send("continue");
    setNormal();
    if (!running) return;
    getRegs();
    getBackTrace();
    if (!running) return;
    getFpRegs();
    getGlobals();
    //if ( running ) emit resetData();
}

void GDB::doStop()
{
    send("kill");
    running = false;
}

void GDB::setBreakpoint(QString file, QString bp)
{
    QStringList results;
    QStringList parts;
    FileLabel f(file, bp);
    if (bpHash.contains(f)) {
        //qDebug() << "bp" << bp << "exists";
        return;
    }
    if (bp[0] == QChar('*')) {
        //qDebug() << "setting" << bp;
        results = sendReceive(QString("break %1").arg(bp));
    } else {
        //qDebug() << QString("break \"%1\":%2").arg(file).arg(bp);
        results = sendReceive(QString("break \"%1\":%2").arg(file).arg(bp));
    }
    foreach ( QString result, results ) {
        parts = result.split(QRegExp("\\s+"));
        if ( parts[0] == "Breakpoint" ) {
            //qDebug() << "sbp" << result << file << bp << parts[1].toInt();
            bpHash[f] = parts[1].toInt();
            break;
        }
    }
}

void GDB::deleteBreakpoint(QString file, QString line)
{
    FileLabel f(file, line);
    int bp = bpHash[f];
    //qDebug() << "dbp" << file << line << bp;
    if (bp > 0) {
        send(QString("delete %1").arg(bp));
        bpHash.remove(f);
    }
}

void GDB::getBackTrace()
{
    QStringList results;
    if (!running) return;
    results = sendReceive("backtrace");
    if ( results.length() == 0 ) results = sendReceive("backtrace");
    //qDebug() << "getBackTrace" << results;
    if (results.length() == 0 || results[0].length() == 0 ||
        results[0][0] != '#') {
        running = false;
        results.clear();
        results.append(tr("Program not running"));
    }
    int n;
    int line;
    QString file;
    QStringList parts;
    QStringList pp;
    foreach ( QString s, results ) {
        //qDebug() << "gbt" << s;
        n = s.lastIndexOf(" at ");
        if ( n > 0 ) {
            file = s.mid(n+4);
            n = file.lastIndexOf(":");
            if ( n < 0 ) continue;
            line = file.mid(n+1).toInt();
            file = file.left(n);
            //qDebug() << "match at" << file << line;
            emit nextInstruction(file,line);
            break;
        } else if ( s.indexOf(" in ") >= 0 ) {
            parts = s.split(QRegExp("\\s+"));
            //dDebug() << parts;
            unsigned long address;
            bool ok;
            address = parts[1].toULong(&ok,16);
            //dDebug() << "address" << address;
            FileLine fl;
            if ( addressToFileLine.contains(address) ) {
                fl = addressToFileLine[address];
                //qDebug() << "ok" << ok << address << fl.file << fl.line;
                emit nextInstruction(fl.file,fl.line);
            } else {
                qDebug() << tr("Could not interpret address:") << address;
            }
            break;
        }
    }
    n = results.length();
    int i;
    for (i = 0; i < n; i++) {
        if (results[i].indexOf("main.") >= 0) break;
    }
    if (i < n) {
        for (i++; i < n; i++) {
            results.removeLast();
        }
    }
    emit sendBackTrace(results);
}

void GDB::getRegs()
{
    QStringList results;
    QStringList parts;
    StringHash map;
    QString s;
    int index1, index2;
    if (!running) return;
    results = sendReceive("info registers");
    //qDebug() << "getRegs" << results;

    foreach ( QString result, results ) {
        parts = result.split(QRegExp("\\s+"));
        //qDebug() << parts;
        if ( regs.contains(parts[0]) ) {
            if ( parts[0] == "eflags" ) {
                //qDebug() << "eflags" << parts;
                index1 = result.indexOf('[');
                index2 = result.indexOf(']');
                if ( index1 < 0 || index2 < 0 ) {
                    s = "";
                    int regdata = parts[2].toInt();
                    for ( int i= 0; i < 7; i++ ) {
                        if ( regdata & reg_masks[i] ) s += reg_names[i] + " ";
                    }
                    map["eflags"] = s;
                } else {
                    s = result.mid(index1+2,index2-index1-3);
                    index1 = s.indexOf("IF");
                    if ( index1 >= 0 ) s.remove(index1,3);
                    map["eflags"] = s;
                }
            } else {
                map[parts[0]] = parts[1];
            }
        }
    }
    //qDebug() << map;
    emit sendRegs(map);
}

void GDB::getFpRegs()
{
    QStringList results;
    QStringList parts;
    QStringList data;
    QString result;
    QRegExp rx1("(0x[0-9A-Fa-f]+).*(0x[0-9A-Fa-f]+).*"
    "(0x[0-9A-Fa-f]+).*(0x[0-9A-Fa-f]+)");
    QRegExp rx2("(0x[0-9A-Fa-f]+).*(0x[0-9A-Fa-f]+)");
    //qDebug() << "getFpRegs";
    bool reverse = ebe["xmm/reverse"].toBool();
    if (!running) return;
    send("set width 0");
    for (int i = 0; i < numFloats; i++) {
        if (hasAVX) {
            results = sendReceive(QString("print/x $ymm%1.v4_int64").arg(i));
            result = "";
            foreach ( QString res, results ) {
                //qDebug() << res;
                //qDebug() << "index" << rx1.indexIn(res);
                if ( rx1.indexIn(res) >= 0 ) {
                    //qDebug() << "res" << rx.cap(1);
                    if ( reverse ) {
                        data.append(rx1.cap(2)+" "+rx1.cap(1)+" "
                            +rx1.cap(4)+" "+rx1.cap(3));
                    } else {
                        data.append(rx1.cap(1)+" "+rx1.cap(2)+" "
                            +rx1.cap(3)+" "+rx1.cap(4));
                    }
                }
            }
        } else {
            results = sendReceive(QString("print/x $xmm%1.v2_int64").arg(i));
            result = "";
            foreach ( QString res, results ) {
                //qDebug() << res;
                //qDebug() << "index" << rx.indexIn(res);
                if ( rx2.indexIn(res) >= 0 ) {
                    //qDebug() << "res" << rx.cap(1);
                    if ( reverse ) {
                        data.append(rx2.cap(2)+" "+rx2.cap(1)+" 0x0 0x0");
                    } else {
                        data.append(rx2.cap(1)+" "+rx2.cap(2)+" 0x0 0x0");
                    }
                }
            }
        }
    }
    send("set width 80");
    emit sendFpRegs(data);
}

void GDB::getGlobals()
{
    VariableDefinitionMap vars;

    //qDebug() << "getGlobals" << globals;
    if (!running) return;
    getVars(globals, vars);
    emit sendGlobals(vars);
}

void GDB::getLocals()
{
    QStringList locals;
    QStringList results;
    QStringList parts;
    VariableDefinitionMap vars;

    if (!running) return;
    results = sendReceive(QString("info locals"));
    foreach ( QString r, results ) {
        int n = r.indexOf(" =");
        if ( n > 0 && r.at(0) != ' ' && r.at(0) != '_' ) {
            locals.append(r.left(n));
        }
    }
    locals.sort();

    getVars(locals, vars);

    emit sendLocals(0,vars);
}

void GDB::getVars(QStringList &names, VariableDefinitionMap &vars)
{
    QStringList results;
    QStringList parts;
    QString cmd;
    QString address;
    Limits dim;
    bool ok;
    bool fortran;
    VariableDefinition var;
    QString baseType;

    if (!running) return;
    foreach ( QString name, names ) {
        fortran = false;
        var.size = 0;
        if ( &names == &globals && name == "stack" ) {
            var.name = name;
            var.type = "unsigned long *";
            var.value = "";
            var.size = 64;
            name = "$rsp";
        } else {
            var.name = name;
            var.type = "";
            var.value = "";
            results = sendReceive(QString("whatis %1").arg(name));
            //qDebug() << "results" << results;
            if ( results.length() > 0 &&
                    (results[0].indexOf("invalid") >= 0 ||
                     results[0].indexOf("(kind") >= 0 ) ) {
                send ( "set language fortran");
                results = sendReceive(QString("whatis %1").arg(name));
                send ( "set language c++");
                fortran = true;
            }
            var.isFortran = fortran;
            foreach ( QString r, results ) {
                //qDebug() << "r" << r;
                int i = r.indexOf("=");
                if ( i > 0 ) {
                    var.type = r.mid(i+2);
                    //qDebug() << name << var.type;
                    break;
                }
            }
        }

        var.dimensions.clear();
        if ( fortran ) {
            parts = var.type.split(QRegExp("\\s+"));
            //qDebug() << "fortran" << parts;
            if ( parts.length() >= 2 ) {
                baseType = parts[0];
                QString d = parts[1];
                d.chop(1);
                d = d.mid(1);
                //qDebug() << var.name << d;
                parts = d.split(",");
                foreach ( QString d2, parts ) {
                    QStringList parts2 = d2.split(":");
                    if ( parts2.length() == 1 ) {
                        dim.first = 1;
                        dim.last = parts2[0].toInt();
                    } else if ( parts2.length() == 2 ) {
                        dim.first = parts2[0].toInt();
                        dim.last = parts2[1].toInt();
                    } else {
                        dim.first = 1;
                        dim.last = 1;
                    }
                    var.dimensions.append(dim);
                }
            }
        } else {
            int n1, n2=-1;
            n1 = var.type.indexOf("[");
            if ( n1 > 0 ) n2 = var.type.indexOf("]");
            var.dimensions.clear();
            while ( n1 > 0 && n2 > 0 ) {
                dim.first = 0;
                dim.last = var.type.mid(n1+1,n2-n1-1).toInt()-1;
                var.dimensions.append(dim);
                n1 = var.type.indexOf("[",n2+1);
                if ( n1 > 0 ) n2 = var.type.indexOf("]",n2+1);
            }
        }
        //qDebug() << "dimensions";

        if ( var.type != "" ) {
            if ( simpleTypes.contains(var.type) ) {
                var.size = sizeForType[var.type];
                QString cmd = QString("x/%1xb &%2")
                    .arg(var.size).arg(name);
                //qDebug() << cmd;
                var.values = sendReceive(cmd);
                //qDebug() << var.values;
            } else if ( var.type.indexOf("character") == 0 ) {
                //qDebug() << name << var.type;
                int i = var.type.indexOf("*");
                if ( i < 0 ) break;
                QString t = var.type.mid(i+1);
                //qDebug() << name << var.type << t;
                int n = t.toInt();
                if ( n > 1000000 ) {
                    cmd = "p _"+name;
                    results = sendReceive(cmd);
                    foreach ( QString r, results ) {
                        parts = r.split(QRegExp("\\s+"));
                        if ( parts.length() == 3 && parts[1] == "=" ) {
                            n = parts[2].toInt();
                            var.type = QString("character*%1").arg(n);
                            break;
                        }
                    }
                }
                if ( n <= 80 ) {
                    results = sendReceive(QString("x/%1xb &%2").arg(n).arg(name));
                    //qDebug() << results;
                    var.value = "\"";
                    foreach ( QString r, results ) {
                        i = r.indexOf(":");
                        if ( i < 0 ) continue;
                        r = r.mid(i+1);
                        parts = r.split(QRegExp("\\s+"));
                        foreach ( QString p, parts ) {
                            if ( p == "" ) continue;
                            n = p.toInt(&ok,16);
                            if ( n >= ' ' && n <= 'z' ) {
                                var.value += QChar(n);
                            } else {
                                var.value += "\\"+p;
                            }
                        }
                    }
                }
                var.value += "\"";
            } else if ( var.type == "std::string" ) {
                var.values.clear();
                //cmd = QString("p %1._M_dataplus._M_p").arg(name);
                cmd = QString("p %1").arg(name);
                //qDebug() << cmd;
                results = sendReceive(cmd);
                //qDebug() << "std::string" << results;
                if ( results.length() == 0 ) continue;
                //parts = results[0].split(QRegExp("\\s+"));
                //if ( parts.length() < 4 ) continue;
                parts = results[0].split(" = ");
                //qDebug() << parts;
                var.values.append(parts[1]);
            } else if ( var.dimensions.length() == 1 ) {
                int n1 = var.dimensions[0].first;
                int n2 = var.dimensions[0].last;
                QString t=var.type;
                int n3;
                if ( isFortran ) {
                    var.size = sizeForType[baseType]*(n2-n1+1);
                } else {
                    n3 = t.indexOf("[");
                    t = t.left(n3).trimmed();
                    var.size = sizeForType[t]*(n2-n1+1);
                }
                QString cmd = QString("x/%1xb %2")
                    .arg(var.size).arg(name);
                //qDebug() << cmd;
                var.values = sendReceive(cmd);
                //qDebug() << var.values;
                var.value = "";
            } else if ( var.type == "char **" ) {
                var.values.clear();
                int i = 0;
                while ( 1 ) {
                    cmd = QString("x/s %1[%2]")
                        .arg(name).arg(i);
                    results = sendReceive(cmd);
                    if ( results.length() == 0 ) break;
                    if ( results[0].indexOf("out of bounds") > 0 ) break;
                    parts = results[0].split(QRegExp(":\\s+"));
                    if ( parts.length() < 2 ) break;
                    if ( parts[0] == "0x0" ) break;
                        var.values += parts[1];
                    i++;
                }
            } else if ( &names == &globals && var.name == "stack" ) {
                QString cmd = QString("x/%1xb %2").arg(var.size).arg(name);
                var.values = sendReceive(cmd);
                //qDebug() << "cmd" << cmd;
                //qDebug() << "results" << results;
                //if ( results.length() == 0 ) {
                //var.value = "";
                //} else {
                //var.value = "";
                //foreach ( QString r, results ) {
                //parts = r.split(QRegExp("\\s+"));
                //for ( int i = 1; i < parts.length(); i++ ) {
                //var.value.append(parts[i]+" ");
                //}
                //}
                //}
            } else if ( var.type.indexOf(" *") >= 0 ) {
                var.size = wordSize/8;
                QString cmd = QString("x/%1xb &%2").arg(var.size).arg(name);
                var.values = sendReceive(cmd);
            } else {
                var.value = "";
            }
            vars[var.name] = var;
        }
    }
}

void GDB::getArgs()
{
    QStringList args;
    QStringList results;
    QStringList parts;
    VariableDefinitionMap vars;

    if (!running) return;
    results = sendReceive(QString("info args"));
    foreach ( QString r, results ) {
        int n = r.indexOf(" =");
        if ( n > 0 && r.at(0) != ' ' && r.at(0) != '_' ) {
            args.append(r.left(n));
        }
    }
    args.sort();
    getVars(args, vars);
    emit sendParameters(0,vars);
}

bool GDB::testAVX()
{
    QStringList results;
    QStringList parts;
    if (!running) return false;
    results = sendReceive("print $ymm0.v4_int64[0]");
    foreach ( QString result, results ) {
        parts = result.split(QRegExp("\\s+"));
        if ( parts.length() == 3 && parts[1] == "=" ) return true;
    }
    return false;
}

void GDB::requestParameters(DataPlank *p, int frame)
{
    QStringList args;
    QStringList results;
    QStringList parts;
    VariableDefinitionMap vars;

    if (!running) return;
    send(QString("frame %1").arg(frame));
    //qDebug() << "requestParameters" << QString("frame %1").arg(frame);
    results = sendReceive(QString("info args"));
    //qDebug() << results;
    foreach ( QString r, results ) {
        int n = r.indexOf(" =");
        if ( n > 0 && r.at(0) != ' ' && r.at(0) != '_' ) {
            args.append(r.left(n));
        }
    }
    //args.sort();
    getVars(args, vars);
    emit sendParameters(p,vars);

    send("frame 0");
}

void GDB::requestLocals(DataPlank *p, int frame)
{
    QStringList locals;
    QStringList results;
    QStringList parts;
    VariableDefinitionMap vars;

    if (!running) return;
    send(QString("frame %1").arg(frame));
    results = sendReceive(QString("info locals"));
    foreach ( QString r, results ) {
        int n = r.indexOf(" =");
        if ( n > 0 && r.at(0) != ' ' && r.at(0) != '_' ) {
            locals.append(r.left(n));
        }
    }
    //locals.sort();
    getVars(locals, vars);
    emit sendLocals(p,vars);

    send("frame 0");
}

void GDB::requestVar(DataPlank *p, QString name, QString address,
        QString type, QString format, int size, int frame)
{
    QStringList results;
    QStringList parts;
    QString result;
    QString cmd;
    QString address2;
    int first = 0;
    int last = 0;

    //qDebug() << "gdb requestVar" << name << address << type << format
             //<< size << frame;
    if (!running) return;

    send(QString("frame %1").arg(frame));
    if (format == "String array") {
        result = "";
        int i = 0;
        while (1) {
            cmd = QString("x/dg ((unsigned char *)%1)+%2").arg(address).arg(
                    i * (wordSize == 64 ? 8 : 4));
            //qDebug() << cmd;
            results = sendReceive(cmd);
            if (results.length() == 0) break;
            parts = results[0].split(QRegExp(":\\s+"));
            if (parts.length() < 2) break;
            address2 = parts[1];
            if (address2 == "0") break;
            cmd = QString("x/sb %1").arg(address2);
            results = sendReceive(cmd);
            if (results.length() == 0) break;
            parts = results[0].split(QRegExp(":\\s+"));
            if (parts.length() < 2) break;
            result += parts[1] + " ";
            i++;
        }
    } else if (format == "String") {
        cmd = QString("x/s (char *)%1").arg(address);
        results = sendReceive(cmd);
        if (results.length() == 0) {
            result = "";
        } else {
            int n = results[0].indexOf("\"");
            result = results[0].mid(n);
        }
    } else if (format == "Pointer") {
        cmd = QString("printf \"0x%x\\n\",%1").arg(address);
        results = sendReceive(cmd);
        if (results.length() == 0) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            result = parts[parts.length() - 1];
        }
    } else {
        cmd = QString("x/%1xb %2").arg(size).arg(address);
        //qDebug() << "cmd" << cmd;
        //qDebug() << results;
        results = sendReceive(cmd);
    }
    send("frame 0");
    emit sendVar(p, name, results);
}

void GDB::getData(QStringList request)
{
    QString name = request[0];
    QString address = request[1];
    QString format = request[2];
    int size = request[3].toInt();
    int first = request[4].toInt();
    int last = request[5].toInt();
    QStringList results;
    QStringList parts;
    QString result;
    QString cmd;
    QString address2;

    if (!running) return;
    //qDebug() << "getData" << request;
    if (size < 0 || size > 8) return;
    char sizeLetter = letterForSize[size];

    char formatLetter = 'd';
    if (format == "Hexadecimal") formatLetter = 'x';
    else if (format == "Floating point") formatLetter = 'f';
    else if (format == "Character") formatLetter = 'c';
    else if (format == "String") formatLetter = 's';

    if (first < 0 || last < 0) return;

    if (format == "String array") {
        result = "";
        int i = 0;
        while (1) {
            cmd = QString("x/dg ((unsigned char *)%1)+%2").arg(address).arg(
                    i * (wordSize == 64 ? 8 : 4));
            //qDebug() << cmd;
            results = sendReceive(cmd);
            //qDebug() << results;
            if (results.length() == 0) break;
            parts = results[0].split(QRegExp(":\\s+"));
            if (parts.length() < 2) break;
            address2 = parts[1];
            if (address2 == "0") break;
            cmd = QString("x/sb %1").arg(address2);
            results = sendReceive(cmd);
            if (results.length() == 0) break;
            parts = results[0].split(QRegExp(":\\s+"));
            if (parts.length() < 2) break;
            result += parts[1] + " ";
            i++;
        }
    } else if (format == "Pointer") {
        cmd = QString("printf \"0x%x\\n\",%1").arg(name);
        results = sendReceive(cmd);
        if (results.length() == 0) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            result = parts[parts.length() - 1];
        }
    } else if (format == "String") {
        cmd = QString("x/s (char *)%1").arg(address);
        //qDebug() << cmd;
        results = sendReceive(cmd);
        //qDebug() << results;
        if (results.length() == 0) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            result = parts[parts.length() - 1];
        }
    } else if (first == 0 && last == 0) {
        cmd = QString("x/x%1 %2").arg(sizeLetter).arg(address);
        results = sendReceive(cmd);
        if (results.length() == 0) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            //qDebug() << "parts" << parts;
            if (parts.length() < 2) {
                result = "";
            } else if (parts[1] == "(gdb)") {
                result = "Illegal address " + parts[0];
            } else {
                result = parts[1];
            }
        }
    } else {
        result = "";
        for (int i = first; i <= last; i++) {
            cmd = QString("x/%1%2 ((unsigned char *)%3)+%4")
                .arg(formatLetter).arg(
                        sizeLetter).arg(address).arg(i * size);
            //qDebug() << cmd;
            results = sendReceive(cmd);
            //qDebug() << results;
            if (results.length() > 0) {
                parts = results[0].split(QRegExp(":\\s+"));
                if (parts.length() >= 2) {
                    result += parts[1] + " ";
                }
            }
        }
    }
    request.append(result);
    emit dataReady(request);
}

void GDB::setNormal()
{
    if (!running) return;
    send("set language c++");
    if (NullEOF) {
        send("call __ebeSetNormal()");
        NullEOF = false;
    }
    emit endFlash();
}

void GDB::setEOF()
{
    if (!running) return;
    send("call __ebeSetNULL()");
    NullEOF = true;
}

void GDB::receiveWorkingDir(QString dir)
{
    QDir::current().cd(dir);
}

void GDB::requestStack(int n)
{
    QStringList results;
    //qDebug() << QString("x/%1xg $rsp").arg(n);
    if (!running) return;
    if ( wordSize == 64 ) {
        results = sendReceive(QString("x/%1xg $rsp").arg(n));
    } else {
        results = sendReceive(QString("x/%1xw $esp").arg(n));
    }
    //qDebug() << results;
    emit receiveStack(results);
}

void GDB::requestAsmVariable(int i, uLong address, int n)
{
    QStringList results;
    if (!running) return;
    results = sendReceive(QString("x/%1xb %2").arg(n).arg(address));
    emit sendAsmVariable(i,results);
}
