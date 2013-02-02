#include <QDebug>
#include <QTimer>
#include <cstdio>
#include "gdb.h"
#include "terminalwindow.h"
#ifdef Q_WS_WIN
#include <windows.h>
#endif

extern TerminalWindow *terminalWindow;
extern int wordSize;

extern GDB *gdb;
QProcess *gdbProcess;

IntHash sizeForType;
char letterForSize[] = "bbhhwwwwg"; 

#ifdef Q_WS_WIN
bool needToKill;
bool needToWake;
HANDLE hProcess;
HANDLE hThread;
#endif

QString readLine()
{
    QString result="";
    int n;
    do {
        while ( gdbProcess->bytesAvailable() == 0 ) {
            gdbProcess->waitForReadyRead(0);
            //qDebug() << "waiting" << gdbProcess->bytesAvailable();
        }
        result += gdbProcess->readLine();
        n = result.length();
        //qDebug() << result;
    } while ( n == 0 || result.at(n-1) != '\n' );
    result.chop(1);
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
    //gdbProcess->setProcessChannelMode(QProcess::MergedChannels);
    gdbProcess->start("gdb");

    //qDebug() << "gdb state" << gdbProcess->state();
    runCommands << "run" << "step" << "next" << "stepi" << "nexti"
                << "continue";
    simpleTypes << "char" << "signed char" << "unsigned char"
                << "short" << "signed short" << "unsigned short"
                << "short int" << "signed short int" << "unsigned short int"
                << "short signed int" << "short unsigned int"
                << "int" << "signed int" << "unsigned int"
                << "signed" << "unsigned"
                << "long" << "signed long" << "unsigned long"
                << "long unsigned" << "long signed"
                << "long unsigned int" << "long signed int"
                << "long int" << "signed long int" << "unsigned long int"
                << "long signed int" << "long unsigned int"
                << "long long" << "signed long long" << "unsigned long long"
                << "bool" << "float" << "double" << "long double";
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
    if ( wordSize == 64 ) {
        numFloats = 16;
        regs << "rax" << "rbx" << "rcx" << "rdx"
             << "rdi" << "rsi" << "rbp" << "rsp"
             << "r8"  << "r9"  << "r10" << "r11"
             << "r12" << "r13" << "r14" << "r15"
             << "rip" << "eflags";
    } else {
        numFloats = 8;
        regs << "eax" << "ebx" << "ecx" << "edx"
             << "edi" << "esi" << "ebp" << "esp"
             << "eip" << "eflags";
    }
    initGdb();
    NullEOF = false;
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
    cmd += '\n';
    gdbProcess->write(cmd.toAscii());
    cmd.chop(1);
    QString result;
    result = readLine();
    //qDebug() << "result:" << result;
    while ( result.left(5) != "(gdb)" ) {
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
        //qDebug() << "result:" << result;
    }
}

QStringList GDB::sendReceive(QString cmd, QString /*options*/)
{
    QStringList list;
    //qDebug() << cmd;
    cmd += '\n';
    gdbProcess->write(cmd.toAscii());
    QString result;
    result = readLine();
    //qDebug() << "result:" << result;
    while ( result.left(5) != "(gdb)" ) {
        list.append(result);
        result = readLine();
        //qDebug() << "result:" << result;
    }
    //qDebug() << list;
    return list;
}

void GDB::getClasses()
{
    QHash<QString,ClassDefinition> classes;
    ClassDefinition c;
    VariableDefinition v;
    QString result;
    QStringList results;
    QString name;
    QStringList parts;
    QStringList names;
    QRegExp rx("^\\s+([a-zA-Z].*)\\s+(\\**)([a-zA-Z0-9][a-zA-Z0-9_]*)(.*);$");

    foreach ( result, classResults ) {
        parts = result.split(QRegExp("\\s+"));
        if ( parts.length() == 1 && parts[0].length() > 0 ) {
            name = parts[0];
            if ( name[name.length()-1] == ';' ) {
                name.chop(1);
                names.append(name);
            }
        } else if ( parts.length() == 2 && parts[0] == "struct" ) {
            name = parts[1];
            if ( name[name.length()-1] == ';' ) {
                name.chop(1);
                names.append(name);
            }
        }
    }
    foreach ( name, names ) {
        //qDebug() << "gc" << name;
        c.name = name;
        c.members.clear();
        results = sendReceive("ptype "+name);
        foreach ( result, results ) {
            if ( result.indexOf("    ") >= 0 && result.indexOf('(') < 0 ) {
                //qDebug() << result;
                if ( rx.indexIn(result) >= 0 ) {
                    //qDebug() << "match" << rx.cap(1) << rx.cap(2) << rx.cap(3) <<
                                //rx.cap(4);
                    v.name = rx.cap(3);
                    v.type = rx.cap(1) + " " + rx.cap(2) + rx.cap(4);
                    if ( v.type[v.type.length()-1] == ' ' ) v.type.chop(1);
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
void GDB::doRun(QString exe, QString options, QStringList files,
          QList<IntSet> breakpoints, QStringList g)
{
    int i;
    int length = files.length();
    globals = g;
    //qDebug() << "length" << length;
    running = false;
    bpHash.clear();
    send("kill");
    send("nosharedlibrary");
    send(QString("cd ")+QDir::currentPath());
    //qDebug() << "file" << exe;
    send("file \""+exe+"\"");
    classResults = sendReceive("info types ^[[:alpha:]][[:alnum:]_]*$");
    send("delete breakpoints");
    for ( i = 0; i < length; i++ ) {
        foreach ( int bp, breakpoints[i] ) {
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
                ws,      // Command line
                NULL,           // Process attributes
                NULL,           // Thread attributes
                TRUE,           // Inherit handles
                flags,          // Creation flags
                NULL,           // Environment
                NULL,           // Current directory
                &si,            // Startup info
                &pi)            // Process information
        ) {
        qDebug() << "Could not create process" << exe;
        DWORD error = GetLastError();
        qDebug() << "error" << error;
        return;
    }
    //qDebug() << "pid" << pi.dwProcessId;
    hProcess = pi.hProcess;
    hThread = pi.hThread;
    needToKill = true;

    send("attach "+QString("%1").arg(pi.dwProcessId));
    //send("delete breakpoints");
    //for ( i = 0; i < length; i++ ) {
        //foreach ( int bp, breakpoints[i] ) {
            ////qDebug() << files[i] << bp;
            ////qDebug() << QString("break %1:%2").arg(files[i]).arg(bp);
            //send(QString("break %1:%2").arg(files[i]).arg(bp) );
        //}
    //}
    send("continue");
    //send("set prompt (gdb)\\n");
#else
    send("tty " + terminalWindow->ptyName);
    send("set args "+options);
    send("run");
#endif
    running = true;
    hasAVX = testAVX();
    setNormal();
    getBackTrace();
    if ( !running ) return;
    getRegs();
    getFpRegs();
    getGlobals();
    getLocals();
    getArgs();
    //qDebug() << "run";
    emit resetData();
    //qDebug() << "run";
    getClasses();
    //qDebug() << "Done run";
}

void GDB::doNext()
{
    //qDebug() << "gdb next";
    if ( !running ) return;
    send("next");
    setNormal();
    getBackTrace();
    if ( !running ) return;
    getRegs();
    getFpRegs();
    getGlobals();
    getLocals();
    getArgs();
    emit resetData();
}

void GDB::doStep()
{
    if ( !running ) return;
    send("step");
    setNormal();
    getBackTrace();
    if ( !running ) return;
    getRegs();
    getFpRegs();
    getGlobals();
    getLocals();
    getArgs();
    emit resetData();
}

void GDB::doContinue()
{
    if ( !running ) return;
    send("continue");
    setNormal();
    getBackTrace();
    if ( !running ) return;
    getRegs();
    getFpRegs();
    getGlobals();
    getLocals();
    getArgs();
    emit resetData();
}

void GDB::doStop()
{
    send("kill");
    running = false;
}

void GDB::setBreakpoint(QString file, int bp)
{
    QStringList results;
    QStringList parts;
    FileLine f(file,bp);
    if ( bpHash.contains(f) ) return;
    results = sendReceive(QString("break \"%1\":%2").arg(file).arg(bp) );
    foreach ( QString result, results ) {
        parts = result.split(QRegExp("\\s+"));
        if ( parts[0] == "Breakpoint" ) {
            //qDebug() << "sbp" << result << file << bp << parts[1].toInt();
            bpHash[f] = parts[1].toInt();
            break;
        }
    }
}

void GDB::deleteBreakpoint(QString file, int line)
{
    FileLine f(file,line);
    int bp = bpHash[f];
    //qDebug() << "dbp" << file << line << bp;
    if ( bp > 0 ) {
        send(QString("delete %1").arg(bp));
        bpHash.remove(f);
    }
}

FileLine::FileLine(QString f, int l)
: file(f), line(l)
{
}

bool FileLine::operator==(FileLine &f) const
{
    return file == f.file && line == f.line;
}

uint qHash(const FileLine &f)
{
    return qHash(f.file) ^ qHash(f.line);
}

void GDB::getBackTrace()
{
    QStringList results;
    results = sendReceive("backtrace");
    //qDebug() << "getBackTrace" << results;
    if ( results.length() == 0 || results[0].length() == 0 ||
         results[0][0] != '#' ) {
        running = false;
        results.clear();
        results.append("Program not running");
    }
    int n;
    int line;
    QString file;
    foreach ( QString s, results ) {
        n = s.indexOf(" at ");
        if ( n > 0 ) {
            file = s.mid(n+4);
            n = file.lastIndexOf(":");
            if ( n < 0 ) continue;
            line = file.mid(n+1).toInt();
            file = file.left(n);
            //qDebug() << "match at" << file << line;
            emit nextInstruction(file,line);
            break;
        }
    }
    emit sendBackTrace(results);
}

void GDB::getRegs()
{
    QStringList results;
    QStringList parts;
    StringHash map;
    int index1, index2;
    results = sendReceive("info registers");
    //qDebug() << "getRegs" << results;

    foreach ( QString result, results ) {
        parts = result.split(QRegExp("\\s+"));
        if ( regs.contains(parts[0]) ) {
            if ( parts[0] == "eflags" ) {
                index1 = result.indexOf('[');
                index2 = result.indexOf(']');
                map[parts[0]] = result.mid(index1+2,index2-index1-3);
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
    for ( int i = 0; i < numFloats; i++ ) {
        if ( hasAVX ) {
            results = sendReceive(QString("print/x $ymm%1.v4_int64").arg(i));
            result = "";
            foreach ( QString res, results ) {
                //qDebug() << res;
                //qDebug() << "index" << rx.indexIn(res);
                if ( rx1.indexIn(res) >= 0 ) {
                    //qDebug() << "res" << rx.cap(1);
                    data.append(rx1.cap(1)+" "+rx1.cap(2)+" "
                               +rx1.cap(3)+" "+rx1.cap(4));
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
                    data.append(rx2.cap(1)+" "+rx2.cap(2)+" 0x0 0x0");
                }
            }
        }
    }
    //qDebug() << data;
    emit sendFpRegs(data);
}


void GDB::getGlobals()
{
    QStringList names;
    QStringList types;
    QStringList values;
    QList<QList<int> > dimensions;

    //qDebug() << "getGlobals" << globals;
    getVars ( globals, names, types, values, dimensions );
    emit sendGlobals(names,types,values);
}

void GDB::getLocals()
{
    QStringList locals;
    QStringList names;
    QStringList types;
    QStringList values;
    QStringList results;
    QStringList parts;
    QList<QList<int> > dimensions;

    results = sendReceive(QString("info locals"));
    //qDebug() << "getLocals" << results;
    foreach ( QString r, results ) {
        //qDebug() << "r" << r;
        int n = r.indexOf(" =");
        if ( n > 0 && r.at(0) != ' ' && r.at(0) != '_' ) {
            locals.append(r.left(n));
        }
    }
    locals.sort();

    getVars ( locals, names, types, values, dimensions );

    //qDebug() << names << types << values;
    emit sendLocals(names,types,values);
}

void GDB::getVars(QStringList &vars, QStringList &names, QStringList &types,
                  QStringList &values, QList<QList<int> > &dimensions )
{
    QStringList results;
    QStringList parts;
    QString type;
    QString name;
    QString value;
    QString cmd;
    QString address;
    QList<int> dim;

    //qDebug() << "names" << vars;
    foreach ( name, vars ) {
        type = "";
        value = "";
        results = sendReceive(QString("whatis %1").arg(name));
        foreach ( QString r, results ) {
            //qDebug() << "r" << r;
            int i = r.indexOf("=");
            if ( i > 0 ) {
                type = r.mid(i+2);
                //qDebug() << name << type;
                break;
            }
        }

        int n1, n2=-1;
        n1 = type.indexOf("[");
        if ( n1 > 0 ) n2 = type.indexOf("]");
        dim.clear();
        while ( n1 > 0 && n2 > 0 ) {
            dim.append(type.mid(n1+1,n2-n1-1).toInt());
            n1 = type.indexOf("[",n2+1);
            if ( n1 > 0 ) n2 = type.indexOf("]",n2+1);
        }
        //qDebug() << "dims" << dim;

        if ( type != "" ) {
            if ( simpleTypes.contains(type) ) {
                char sizeLetter = letterForSize[sizeForType[type]];
                QString cmd = QString("x/%1 (unsigned char *)&%2").arg(sizeLetter).arg(name);
                //qDebug() << cmd;
                results = sendReceive(cmd);
                //qDebug() << results;
                foreach ( QString r, results ) {
                    //qDebug() << r;
                    parts = results[0].split(QRegExp(":\\s+"));
                    if ( parts.length() == 2 ) {
                        value = parts[1];
                        break;
                    }
                }
            } else if ( type == "std::string" ) {
                value = "";
                cmd = QString("p %1._M_dataplus._M_p").arg(name);
                //qDebug() << cmd;
                results = sendReceive(cmd);
                //qDebug() << results;
                if ( results.length() == 0 ) continue;
                parts = results[0].split(QRegExp("\\s+"));
                if ( parts.length() < 4 ) continue;
                value = parts[3];
            } else if ( dim.length() == 1 ) {
                int n = dim[0];
                if ( n > 10 ) n = 10;
                value = "";
                for ( int i = 0; i < n; i++ ) {
                    results = sendReceive(QString("print %1[%2]").arg(name).arg(i));
                    //qDebug() << name << results;
                    foreach ( QString r, results ) {
                        int j = r.indexOf("=");
                        if ( j > 0 ) {
                            value += r.mid(j+2) + " ";
                            break;
                        }
                    }
                }
            } else if ( type == "char **" ) {
                value = "";
                int i = 0;
                while ( 1 ) {
                    cmd = QString("x/dg ((unsigned char *)%1)+%2").arg(name).arg(i*8);
                    //qDebug() << cmd;
                    results = sendReceive(cmd);
                    //qDebug() << results;
                    if ( results.length() == 0 ) break;
                    parts = results[0].split(QRegExp(":\\s+"));
                    if ( parts.length() < 2 ) break;
                    address = parts[1];
                    if ( address == "0" ) break;
                    cmd = QString("x/sb %1").arg(address);
                    results = sendReceive(cmd);
                    if ( results.length() == 0 ) break;
                    parts = results[0].split(QRegExp(":\\s+"));
                    if ( parts.length() < 2 ) break;
                    value += parts[1] + " ";
                    i++;
                }
            } else if ( type.indexOf(" *") >= 0 ) {
                QString cmd = QString("printf \"0x%x\\n\",%1").arg(name);
                results = sendReceive(cmd);
                if ( results.length() == 0 ) {
                    value = "";
                } else {
                    parts = results[0].split(QRegExp(":\\s+"));
                    value = parts[parts.length()-1];
                }
            } else {
                value = " ";
            } 
            names.append(name);
            types.append(type);
            values.append(value);
            dimensions.append(dim);
        }
    }
}

void GDB::getArgs()
{
    QStringList args;
    QStringList names;
    QStringList types;
    QStringList values;
    QStringList results;
    QStringList parts;
    QList<QList<int> > dimensions;
    QString type;
    QString name;
    QString value;

    results = sendReceive(QString("info args"));
    //qDebug() << "getArgs" << results;
    foreach ( QString r, results ) {
        //qDebug() << "r" << r;
        int n = r.indexOf(" =");
        if ( n > 0 && r.at(0) != ' ' && r.at(0) != '_' ) {
            args.append(r.left(n));
        }
    }
    args.sort();
    getVars ( args, names, types, values, dimensions );
    //qDebug() << "parameters" << names << types << values;
    emit sendParameters(names,types,values);
}


bool GDB::testAVX()
{
    QStringList results;
    QStringList parts;
    results = sendReceive("print $ymm0.v4_int64[0]");
    //qDebug() << "testAVX" << results;
    foreach ( QString result, results ) {
        parts = result.split(QRegExp("\\s+"));
        if ( parts.length() == 3 && parts[1] == "=" ) return true;
    }
    return false;
}

void GDB::requestVar(DataMap *map, QString name, QString address, QString type,
                     QString format, int size, int first, int last)
{
    QStringList results;
    QStringList parts;
    QString result;
    QString cmd;
    QString address2;

    if ( size < 0 || size > 8 ) return;
    char sizeLetter = letterForSize[size];
 
    char formatLetter = 'd';
    if ( format == "Hexadecimal" ) formatLetter = 'x';
    else if ( format == "Floating point" ) formatLetter = 'f';
    else if ( format == "Character" ) formatLetter = 'c';

    if ( first < 0 || last < 0 ) return;

    if ( format == "String array" ) {
        result = "";
        int i = 0;
        while ( 1 ) {
            cmd = QString("x/dg ((unsigned char *)%1)+%2").arg(address).arg(i*8);
            //qDebug() << cmd;
            results = sendReceive(cmd);
            //qDebug() << results;
            if ( results.length() == 0 ) break;
            parts = results[0].split(QRegExp(":\\s+"));
            if ( parts.length() < 2 ) break;
            address2 = parts[1];
            if ( address2 == "0" ) break;
            cmd = QString("x/sb %1").arg(address2);
            results = sendReceive(cmd);
            if ( results.length() == 0 ) break;
            parts = results[0].split(QRegExp(":\\s+"));
            if ( parts.length() < 2 ) break;
            result += parts[1] + " ";
            i++;
        }
    } else if ( format == "Pointer" ) {
        cmd = QString("printf \"0x%x\\n\",%1").arg(name);
        results = sendReceive(cmd);
        if ( results.length() == 0 ) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            result = parts[parts.length()-1];
        }
    } else if ( first == 0 && last == 0 ) {
        cmd = QString("x/x%1 %2").arg(sizeLetter).arg(address);
        results = sendReceive(cmd);
        if ( results.length() == 0 ) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            if ( parts.length() < 2 ) {
                result = "";
            } else {
                result = parts[1];
            }
        }
    } else {
        result = "";
        for ( int i = first; i <= last; i++ ) {
            cmd = QString("x/%1%2 ((unsigned char *)%3)+%4").arg(sizeLetter).arg(formatLetter).arg(name).arg(i*size);
            results = sendReceive(cmd);
            if ( results.length() > 0 ) {
                parts = results[0].split(QRegExp(":\\s+"));
                if ( parts.length() >= 2 ) {
                    result += parts[1] + " ";
                }
            }
        }
    }
    emit sendVar ( map, name, result );
}

void GDB::getData(QStringList request)
{
    QString name = request[0];
    QString address = request[1];
    QString format = request[2];
    int size = request[3].toInt();
    int first = request[4].toInt();
    int last  = request[5].toInt();
    QStringList results;
    QStringList parts;
    QString result;
    QString cmd;
    QString address2;


    if ( size < 0 || size > 8 ) return;
    char sizeLetter = letterForSize[size];

    char formatLetter = 'd';
    if ( format == "Hexadecimal" ) formatLetter = 'x';
    else if ( format == "Floating point" ) formatLetter = 'f';
    else if ( format == "Character" ) formatLetter = 'c';
    else if ( format == "String" ) formatLetter = 's';

    if ( first < 0 || last < 0 ) return;

    if ( format == "String array" ) {
        result = "";
        int i = 0;
        while ( 1 ) {
            cmd = QString("x/dg ((unsigned char *)%1)+%2").arg(address).arg(i*8);
            //qDebug() << cmd;
            results = sendReceive(cmd);
            //qDebug() << results;
            if ( results.length() == 0 ) break;
            parts = results[0].split(QRegExp(":\\s+"));
            if ( parts.length() < 2 ) break;
            address2 = parts[1];
            if ( address2 == "0" ) break;
            cmd = QString("x/sb %1").arg(address2);
            results = sendReceive(cmd);
            if ( results.length() == 0 ) break;
            parts = results[0].split(QRegExp(":\\s+"));
            if ( parts.length() < 2 ) break;
            result += parts[1] + " ";
            i++;
        }
    } else if ( format == "Pointer" ) {
        cmd = QString("printf \"0x%x\\n\",%1").arg(name);
        results = sendReceive(cmd);
        if ( results.length() == 0 ) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            result = parts[parts.length()-1];
        }
    } else if ( first == 0 && last == 0 ) {
        cmd = QString("x/x%1 %2").arg(sizeLetter).arg(address);
        results = sendReceive(cmd);
        if ( results.length() == 0 ) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            if ( parts.length() < 2 ) {
                result = "";
            } else {
                result = parts[1];
            }
        }
    } else {
        result = "";
        for ( int i = first; i <= last; i++ ) {
            cmd = QString("x/%1%2 ((unsigned char *)%3)+%4").arg(formatLetter).arg(sizeLetter).arg(address).arg(i*size);
            //qDebug() << cmd;
            results = sendReceive(cmd);
            //qDebug() << results;
            if ( results.length() > 0 ) {
                parts = results[0].split(QRegExp(":\\s+"));
                if ( parts.length() >= 2 ) {
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
    if ( NullEOF ) {
        send("call __ebeSetNormal()");
        NullEOF = false;
    }
    emit endFlash();
}

void GDB::setEOF()
{
    send("call __ebeSetNULL()");
    NullEOF = true;
}

void GDB::receiveWorkingDir(QString dir)
{
    QDir::current().cd(dir);
}
