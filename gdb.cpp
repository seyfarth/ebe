#include <QDebug>
#include <QTimer>
#include <cstdio>
#include "gdb.h"
#include "settings.h"
#include "terminalwindow.h"
#ifdef Q_OS_WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

extern TerminalWindow *terminalWindow;
extern int wordSize;
extern QMap<FileLine, unsigned long> fileLineToAddress;
extern QHash<unsigned long, FileLine> addressToFileLine;
extern QString breakFile;
extern int breakLine;
extern QString dbgName;


extern Debugger *dbg;

bool d=false;
bool stopped;
bool starting=false;

//static QSemaphore critSem(1);
static QSemaphore gateSem(1);
//static QSemaphore resultSem(0);
//static QSemaphore launchSem(0);

static volatile bool resultReady;

QString currentLanguage;

GDB *gdb;
GDBThread *gdbThread;

GDBThread::GDBThread()
    : QThread()
{
    gdbThread = this;
}

GDBReaderThread::GDBReaderThread()
    : QThread()
{
}

void GDBReaderThread::run()
{
    //qDebug() << "reader thread" << currentThreadId();
    //qDebug() << "in reader" << dbg->dbgProcess->state()
             //<< dbg->dbgProcess->pid();
    //connect ( gdb->dbgProcess, SIGNAL(readyReadStandardOutput()),
              //this, SLOT(handleRead()) );
    exec();
}

void GDBReaderThread::handleRead()
{
    //gdb->handleRead();
}

void GDBThread::run()
{
    //qDebug() << "thread" << currentThreadId();
    dbg = gdb = new GDB;
    dbg->initDBG();
    Debugger::runningStatus = Debugger::dbgProcess->state();
    exec();
}

GDB::GDB()
    : Debugger()
{
#if defined(Q_OS_WIN32)
    needToKill = false;
#endif
    dbgProcess = new QProcess(this);
    dbgProcess->setProcessChannelMode(QProcess::MergedChannels);
    dbgProcess->start(dbgName + " -n", QIODevice::ReadWrite | QIODevice::Text );

    wordSize = ebe["build/word_size"].toInt();
    //qDebug() << "gdb state" << dbgProcess->state() << dbgProcess->pid();
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
    NullEOF = false;
    //reader = new GDBReaderThread();
    //reader->start();
    this->moveToThread(gdbThread);
    //qDebug() << "reader" << reader;
}

void GDB::send(QString cmd)
{
    sendReceive ( cmd );
}

void GDB::sendRaw(QString cmd)
{
    sendReceive ( cmd );
}

static QStringList hrResults;

QStringList GDB::sendReceive(QString cmd)
{
    QString result;
    QStringList list;

    //if ( cmd.left(5) == "break" ) qDebug() << "sendReceive start" << cmd;
    stopped = !((cmd.left(3) == "run") || (cmd == "continue") || (cmd == "r"));
    //acquireSem(critSem);
    writeLine ( cmd );
    emit log(cmd);
    //qDebug() << "SR waiting";
    //acquireSem(resultSem);
    handleRead();
    //if ( cmd.left(5) == "break" ) qDebug() << "SR results" << hrResults;
    list = hrResults;
    hrResults.clear();
    foreach ( result, list ) emit log(result);
    //releaseSem(critSem);
    return list;
}

QStringList GDB::splitLines(QString s)
{
    s = s.replace("\"","");
    s = s.replace(QRegExp("\\\\n$"),"");
    return s.split("\\n");
}

void GDB::handleRead()
{
    QString result;
    QString res;
    QStringList results;
    QStringList parts;

    //qDebug() << "Entered handleRead, stopped" << stopped;

    resultReady = false;
    while ( !(resultReady && stopped) ) {
        if ( dbgProcess->bytesAvailable() < 1 ) dbgProcess->waitForReadyRead(-1);
        res = dbgProcess->readLine();
        //qDebug() << "res" << res;
        //if ( res.left(5) == "(gdb)" ) continue;
        //if ( res.left(5) == "mxcsr" ) continue;
        result += res;
        //qDebug() << "res.right(1)" << res.right(1);
        if ( res.right(1) != "\n" ) continue;
        //qDebug() << "handleRead" << result;
        result = result.replace("\"","");
        result = result.replace("\t"," ");
        result = result.replace("\n"," ");
        //result = result.replace("\"","");
        result = result.replace(QRegExp(" *$"),"");
        //qDebug() << "handleRead fixed" << result;
        emit log(result);
        //if ( result.indexOf("Temporary breakpoint") >= 0 ) {
            //qDebug() << "Temporary breakpoint";
            //continue;
        //}
        //if ( result.indexOf(QRegExp("^0x[0-9a-fA-f]+ in ")) == 0 ) {
            //qDebug() << "Hex address in";
            //continue;
        //}
        if ( (result.left(4) == "done") || (result.left(5) == "error") ) {
            if ( result.left(5) == "error" ) stopped = true;
                //resultSem.release(1);
                //qDebug() << "finished previous command";
                //QCoreApplication::processEvents(QEventLoop::AllEvents,0);
        }
        if ( result.indexOf("libc_start_main") >= 0 ) {
               //qDebug() << "libc_start_main" << result;
        }

        int locBreakpoint = result.indexOf(QRegExp("Breakpoint [0-9]+\\,"));
        //qDebug() << "locBreakpoint" << locBreakpoint;
        if ( locBreakpoint >= 0 ) {
            stopped = true;
       } else if ( result.indexOf("aborted") >= 0 ) {
            //qDebug() << "aborted";
            stopped = true;
            running = false;
            QStringList results;
            results.append(tr("Program not running"));
            emit sendBackTrace(results);
        } else {
            locBreakpoint = result.indexOf(QRegExp("Temporary breakpoint [0-9]+\\,"));
            //qDebug() << "locBreakpoint" << locBreakpoint;
            if ( locBreakpoint >= 0 ) {
                stopped = true;
                result = result.replace("Temporary b","B");
            }
        }

        if ( stopped && (result.indexOf(QRegExp("0x[0-9A-Fa-f]+ in ")) == 0) ) {
            //qDebug() << "calling ni" << result;
            handleNextInstruction(result);
        } else if ( stopped && ((locBreakpoint >= 0) ||
              (result.indexOf("step") >= 0)) ) {
            //qDebug() << "calling ni" << result;
            handleNextInstruction(result);
        } else if ( (result.indexOf("exited") >= 0) ||
                    (result.indexOf("killed") >= 0) ||
                    (result.indexOf("no longer exists") >= 0) ) {
            //qDebug() << "exited" << result;
            stopped = true;
            running = false;
            QStringList results;
            results.append(tr("Program not running"));
            emit sendBackTrace(results);
        }
        if ( res.indexOf("(gdbdone)") >= 0 ) {
            resultReady = true;
            //qDebug() << "gdbdone at" << res.indexOf("(gdbdone)");
        } else {
            hrResults += result;
            result = "";
        }
    }
    while ( dbgProcess->bytesAvailable() > 0 ) {
        res = dbgProcess->readLine();
    }
    //qDebug() << "exit handleRead";
}

void GDB::handleNextInstruction(QString result)
{
    QStringList parts;
    QStringList parts2;
    QString file;
    int line=-1;
    int n;
    unsigned long address;
    bool ok;

    //if ( result.indexOf(QRegExp("func=.?start.?,")) >= 0 ) return;
    if ( result.indexOf("libc_start_main") >= 0 ) return;
    parts = result.split(", ");
    //qDebug() << "handleNextInstruction" << parts;
    n = parts.length();
    if ( n >= 2 ) {
        parts2 = parts[1].split(" ");
        //qDebug() << "handleNextInstruction parts2" << parts2;
        if ( parts2.length() >= 1 ) address = parts2[0].toULong(&ok,16);
    } else if ( n == 1 ) {
        parts2 = parts[0].split(" ");
        //qDebug() << "handleNextInstruction parts2" << parts2;
        if ( parts2.length() >= 1 ) address = parts2[0].toULong(&ok,16);
    }

    if ( file == "" ) {
        currentLanguage = "c++";
    } else if ( file.indexOf(QRegExp(".c$")) >= 0 ) {
        currentLanguage = "c";
    } else {
        currentLanguage = "c++";
    }
    if ( file == "" ) {
        FileLine fl;
        if ( addressToFileLine.contains(address) ) {
            fl = addressToFileLine[address];
            //qDebug() << address << fl.file << fl.line;
            emit nextInstruction(fl.file,fl.line);
        } else if ( !starting ) {
            qDebug() << tr("Could not interpret address:")
                     << QString("0x%1").arg(address,0,16);
        }
        //qDebug() << "stopped" << address << file << line;
    } else {
        emit nextInstruction(file,line);
        //qDebug() << "stopped" << address << file << line << currentLanguage;
    }
}

void GDB::completeStep()
{
    //qDebug() << "completeStep start";
    acquireSem(gateSem);
    //qDebug() << "completeStep start 2";
    if ( running ) setNormal();
    //qDebug() << "completeStep start 3";
    if ( running ) getClasses();
    //qDebug() << "completeStep start 4";
    if ( running ) getRegs();
    //qDebug() << "completeStep start 5";
    if ( running ) getFpRegs();
    //qDebug() << "completeStep start 6";
    if ( running ) getBackTrace();
    //qDebug() << "completeStep start 7";
    if ( running ) getGlobals();
    //qDebug() << "completeStep start 8";
    releaseSem(gateSem);
    //qDebug() << "completeStep start 9";
    if ( running ) emit resetData();
    //qDebug() << "completeStep end";
}

void GDB::processTypedefRequest(QString name, QString &type)
{
    acquireSem(gateSem);
    d = false;
    VariableDefinition v;
    QString result;
    QStringList results;
    QStringList parts;
    QRegExp rx("^\\s+([a-zA-Z].*)\\s+(\\**)([a-zA-Z0-9][a-zA-Z0-9_]*)(.*);$");
    //qDebug() << "processTypedefRequest" << name;
    latestClass.name = name;
    latestClass.members.clear();
    results = sendReceive("whatis " + name);

    foreach(result, results)
    {
        //qDebug() << result;
        result = result.replace(" \"","");
        //qDebug() << result;
        parts = result.split(" = ");
        //qDebug() << parts;
        if ( parts.length() == 2 ) type = parts[1];
    }
    d = false;
    releaseSem(gateSem);
}

void GDB::processClassRequest(QString name, ClassDefinition &c)
{
    acquireSem(gateSem);
    QRegExp rx("^\\s+([a-zA-Z].*)\\s+(\\**)([a-zA-Z0-9][a-zA-Z0-9_]*)(.*);$");
    QString result;
    QStringList results;
    VariableDefinition v;
    c.name = name;
    c.members.clear();
    results = sendReceive("ptype " + name);
    foreach(result, results)
    {
        if (result.indexOf("    ") >= 0 && result.indexOf('(') < 0) {
            //qDebug() << result;
            if (rx.indexIn(result) >= 0) {
                //qDebug() << "match" << rx.cap(1) << rx.cap(2) << rx.cap(3) << rx.cap(4);
                v.name = rx.cap(3);
                v.type = rx.cap(1) + " " + rx.cap(2) + rx.cap(4);
                if (v.type[v.type.length() - 1] == ' ') v.type.chop(1);
                v.isSimple = simpleTypes.contains(v.type);
                //qDebug() << v.name << v.type << v.isSimple;
                c.members.append(v);
            }
        }
    }
    releaseSem(gateSem);
}

void GDB::getClasses()
{
    QHash < QString, ClassDefinition > classes;
    ClassDefinition c;
    VariableDefinition v;
    QString result;
    QString s;
    QString t;
    QStringList results;
    QStringList res2;
    QStringList res3;
    QString name;
    QStringList parts;
    QStringList names;
    QStringList newTypes;
    QRegExp rx("^\\s+([a-zA-Z].*)\\s+(\\**)([a-zA-Z0-9][a-zA-Z0-9_]*)(.*);$");
    int n;
    int frame;

    d = false;
    results = sendReceive("bt");
    //qDebug() << "getClasses" << results;
    int i;
    n = results.length();
    for (i = 0; i < n; i++) {
        if (results[i].indexOf("main.") >= 0) break;
    }
    if ( i == n ) i = 0;
    for (i++; i < n; i++) {
        results.removeLast();
    }
    //qDebug() << "getClasses" << results;
    n = results.length();
    if ( n < 1 ) {
        releaseSem(gateSem);
        d = false;
        return;
    }

    //qDebug() << "globals" << globals;
    foreach (s, globals ) {
        if ( s != "stack" ) {
            res2 = sendReceive("whatis ::" + s );
            if ( res2.length() > 0 ) {
                parts = res2[0].split(" = ");
                if ( parts.length() > 1 ) {
                    t = parts[1];
                    t = t.replace("volatile ","");
                    t = t.replace("const ","");
                    t = t.replace(" const","");
                    t = t.replace("static ","");
                    t = t.replace(QRegExp(" \\*+"),"");
                    t = t.replace(QRegExp("\\[.*"),"");
                    newTypes << t;
                }
            }
        }
    }
    //qDebug() << "global types" << newTypes;

    frame = 0;
    foreach ( result, results ) {
        if ( result.indexOf(" at ") < 0 ) {
            frame++;
            continue;
        }
        send(QString("frame %1").arg(frame));
        res2 = sendReceive("info args");
        //qDebug() << "frame" << frame << res2;
        foreach ( s, res2 ) {
            if ( s == "" || s[0] == ',' || s[0] == '}' ) continue;
            parts = s.split(" = ");
            //qDebug() << "parts" << parts;
            name  = parts[0];
            res3 = sendReceive("whatis "+name);
            if ( res3.length() > 0 ) {
                parts = res3[0].split(" = ");
                if ( parts.length() > 1 ) {
                    t = parts[1];
                    t = t.replace("volatile ","");
                    t = t.replace("const ","");
                    t = t.replace(" const","");
                    t = t.replace("static ","");
                    t = t.replace(QRegExp(" \\*+"),"");
                    t = t.replace(QRegExp("\\[.*"),"");
                    newTypes << t;
                }
            }
        }
        //qDebug() << "newTypes with args" << newTypes;
        res2 = sendReceive("info locals");
        //qDebug() << "frame" << frame << res2;
        foreach ( s, res2 ) {
            if ( s == "" || s[0] == ',' || s[0] == '}' ) continue;
            parts = s.split(" = ");
            //qDebug() << "parts" << parts;
            name  = parts[0];
            res3 = sendReceive("whatis "+name);
            if ( res3.length() > 0 ) {
                parts = res3[0].split(" = ");
                if ( parts.length() > 1 ) {
                    t = parts[1];
                    t = t.replace("volatile ","");
                    t = t.replace("const ","");
                    t = t.replace(" const","");
                    t = t.replace("static ","");
                    t = t.replace(QRegExp(" \\*+"),"");
                    t = t.replace(QRegExp("\\[.*"),"");
                    newTypes << t;
                }
            }
        }
        //qDebug() << "newTypes with locals" << newTypes;
        if ( result.indexOf("`main") >= 0 ) break;
        frame++;
    }
        
    //qDebug() << "getClasses new types" << newTypes;

    while ( !newTypes.empty() ) {
        t = newTypes.first();
        //qDebug() << "first" << t;
        newTypes.removeFirst();
        if ( classes.count(t) > 0 ) continue;
        if ( simpleTypes.contains(t) ) continue;
        c.name = t;
        c.members.clear();
        results = sendReceive("ptype " + t );
        QString firstT = t;
        while ( results.length() == 1 ) {
            t = results[0];
            t = t.replace("volatile ","");
            t = t.replace("const ","");
            t = t.replace(" const","");
            t = t.replace("static ","");
            t = t.replace(QRegExp(" \\*+"),"");
            t = t.replace(QRegExp("\\[.*"),"");
            if ( t == firstT || simpleTypes.contains(t) || classes.count(t) > 0 ) {
                continue;
            }
            results = sendReceive("ptype " + t );
        } 
        if ( results.length() == 1 ) continue;
        bool inMembers = false;
        QString r;
        int k, j, i;
        QString arraySpec;
        QString memberName;
        QString memberType;
        QRegExp rx1("^[\\s]*([^\\[]*)(\\[[^;]*);?$");
        //QRegExp rx2("^ +(.+[ \\*])([a-zA-Z_0-9]+)$");
        //
        // find struct/class members for t
        //
        foreach ( r, results ) {
            //qDebug() << "proc" << r;
            if ( r[0] == QChar(' ') ) {
                n = r.length();
                //
                // check characters from right going past
                // array brackets
                //
                arraySpec = "";
                if ( rx1.exactMatch(r) ) {
                    //qDebug() << r << rx1.cap(1) << rx1.cap(2);
                    r = rx1.cap(1);
                    arraySpec = rx1.cap(2);
                } else {
                    arraySpec = "";
                }
                //qDebug() << "type/name" << r << "array" << arraySpec;
                i = r.lastIndexOf(" ");
                j = r.lastIndexOf("*");
                k = r.lastIndexOf("&");
                //qDebug() << i << j << k;
                if ( j > i ) i = j;
                if ( k > i ) i = k;
                memberName = r.mid(i+1);
                memberType = r.left(i+1);
                memberType = memberType.replace(QRegExp("^[\\s]+"),"");
                memberType = memberType.replace(QRegExp("[\\s]+$"),"");
                if ( memberName.right(1) == ";" ) memberName.chop(1);
                if ( arraySpec != 0 ) memberType += " " + arraySpec;
                //qDebug() << memberType << memberName;
                v.name = memberName;
                v.type = memberType;
                v.isSimple = simpleTypes.contains(v.type);
                c.members.append(v);
                inMembers = true;
                memberType = memberType.replace(QRegExp("[\\[\\*].*$"),"");
                memberType = memberType.replace(QRegExp(" +$"),"");
                if ( !simpleTypes.contains(memberType) &&
                     classes.count(memberType) == 0 ) {
                    //qDebug() << "new type" << memberType;
                    newTypes << memberType;
                }
            } else if ( inMembers ) {
                break;
            }
        }
        //foreach ( v, c.members ) {
            //qDebug() << "   " << t << v.name << v.type;
        //}
        classes[t] = c;
    }

    //qDebug() << "classes done";
    emit sendClasses(classes);
    d = false;
}

void GDB::initDBG()
{
    running = false;
    dbgProcess->setTextModeEnabled(true);
    dbgProcess->setReadChannel(QProcess::StandardOutput);
    send("set prompt \"(gdbdone)\\n(gdb)\"");
    send("show prompt");
    send("maintenance set target-async off");
}

//public slots:
void GDB::doCommand(QString cmd, QString)
{
    acquireSem(gateSem);
    send(cmd);
    releaseSem(gateSem);
}

void GDB::doRun(QString exe, QString options, QStringList files,
    QList<StringSet> breakpoints, QStringList g)
{
    int i;
    int length = files.length();

    acquireSem(gateSem);
    //isFortran = false;
    globals = g;
    globals.append("stack");
    //qDebug() << "length" << length;
    running = false;
    bpHash.clear();
    send("set print repeats 0");
    send("set height 0"); 
    send("set width 100000"); 
    send("kill");
    if ( gdb->dbgProcess->state() != QProcess::Running ) {
        emit error("gdb is not running");
        releaseSem(gateSem);
        return;
    }
    send("nosharedlibrary");
    send(QString("cd ") + QDir::currentPath());
    //qDebug() << "file" << exe;
    send("file \"" + exe + "\"");
    send("delete breakpoints");
    for (i = 0; i < length; i++) {
        foreach ( QString bp, breakpoints[i] ) {
            //qDebug() << files[i] << bp;
            setBreakpointInternal(files[i],bp);
        }
    }

#if defined(Q_OS_WIN32)
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
        releaseSem(gateSem);
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
    hasAVX = testAVX();
    send("continue");
    //send("set prompt (gdb)\\n");
#endif
    //qDebug() << "tty " << terminalWindow->ptyName;
#ifdef Q_OS_MAC
    send("set startup-with-shell off");
#endif
#if defined(Q_OS_WIN32)
    if ( ebe["terminal/use"].toBool() ) {
        send ( "set new-console on" );
    } else {
        send ( "set new-console off" );
    }
#else
    if ( ebe["terminal/use"].toBool() ) {
        send("set inferior-tty " + ebe["terminal/name"].toString() );
    } else {
        send("set inferior-tty " + terminalWindow->ptyName);
    }
#endif
    starting = true;
#if defined(Q_OS_MAC)
    send("b start");
#elif not defined(Q_OS_WIN32)
    send("b _start");
#endif
    send("run " + options);
#if defined(Q_OS_MAC)
    hasAVX = testAVX();
    send("continue");
#elif defined(Q_OS_WIN32)
    hasAVX = testAVX();
    send("continue");
#else
    hasAVX = testAVX();
    QStringList results = sendReceive("info inferiors");
    extern int targetPid;
    targetPid = 0;
    foreach ( QString result, results ) {
        if ( result[0] == QChar('*') ) {
            QStringList parts = result.split(QRegExp(" +"));
            if ( parts.length() >= 5 ) targetPid = parts[3].toInt();
            break;
        }
    }
    send("continue");
    starting = false;
#endif
    running = true;
    setNormal();
    //qDebug() << "run";
    //qDebug() << "Done run";
    releaseSem(gateSem);
    if ( running ) emit resetData();
}

void GDB::doNext()
{
    //qDebug() << "gdb next" << running;
    if (!running) return;
    acquireSem(gateSem);
    send("next");
    releaseSem(gateSem);
}

void GDB::doStep()
{
    //qDebug() << "gdb step";
    //qDebug() << "gdb step" << running;
    if (!running) return;
    acquireSem(gateSem);
    send("step");
    releaseSem(gateSem);
}

void GDB::doNextInstruction()
{
    //qDebug() << "gdb nexti" << running;
    if (!running) return;
    acquireSem(gateSem);
    send("nexti");
    releaseSem(gateSem);
}

void GDB::doStepInstruction()
{
    //qDebug() << "gdb stepi" << running;
    //qDebug() << "stepi";
    if (!running) return;
    acquireSem(gateSem);
    send("stepi");
    releaseSem(gateSem);
}

void GDB::doCall()
{
    FileLine flx(breakFile, breakLine);
    FileLine fl(breakFile, breakLine + 1);
    //qDebug() << "current" << fileLineToAddress[flx] << "   next" << fileLineToAddress[fl];
    //qDebug() << "call from" << breakFile << breakLine;
    //for ( int i = 1; i < 100; i++ ) {
        //FileLine fl2(breakFile, i );
        //qDebug() << "fl2" << i << fileLineToAddress[fl2];
    //}
    if (!running) return;
    acquireSem(gateSem);
    if ( ebe["build/assembler"].toString() == "nasm" ) {
        //qDebug() << "tbreak *" << fileLineToAddress[fl] << "OK";
        send(QString("tbreak *%1").arg(fileLineToAddress[fl]));
    } else {
        send(QString("tbreak \"%1\":%2").arg(breakFile).arg(breakLine+1));
    }
//#endif
    send("continue");
    releaseSem(gateSem);
}

void GDB::doContinue()
{
    //qDebug() << "continue";
    if (!running) return;
    acquireSem(gateSem);
    send("continue");
    releaseSem(gateSem);
}

void GDB::doStop()
{
    acquireSem(gateSem);
    send("kill");
    running = false;
    releaseSem(gateSem);
}

void GDB::setBreakpoint(QString file, QString bp)
{
    acquireSem(gateSem);
    setBreakpointInternal(file,bp);
    releaseSem(gateSem);
}

void GDB::setBreakpointInternal(QString file, QString bp)
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

void GDB::requestReset()
{
    if ( running ) emit resetData();
}

void GDB::deleteBreakpoint(QString file, QString line)
{
    acquireSem(gateSem);
    FileLabel f(file, line);
    int bp = bpHash[f];
    //qDebug() << "dbp" << file << line << bp;
    if (bp > 0) {
        send(QString("delete %1").arg(bp));
        bpHash.remove(f);
    }
    releaseSem(gateSem);
}

void GDB::getBackTrace()
{
    QStringList results;
    if (!running) return;
    results = sendReceive("backtrace");
    if ( results.length() == 0 ) results = sendReceive("backtrace");
    //qDebug() << "getBackTrace" << results;
    if (results.length() == 0 || results[0].length() == 0 ||
        results[0][0] != '#'  || results[0].indexOf("libc_start_main") >= 0 ) {
        //running = false;
        results.clear();
        results.append(tr("Program not running"));
        send("kill");
        emit sendBackTrace(results);
    }
    int n;
    QString file;
    QStringList parts;
    QStringList pp;
    n = results.length();
    int i;
    for (i = 0; i < n; i++) {
        if (results[i].indexOf("main.") >= 0) break;
    }
    if ( i == n ) i = 0;
    for (i++; i < n; i++) {
        results.removeLast();
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
        //result = result.replace("\"","");
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
    QString part;
    QString result;
    QString reg;
    QString s;
    QString t;
    QString value;
    int i, j, k, n, n2;
    QRegExp rx1("(0x[0-9A-Fa-f]+).*(0x[0-9A-Fa-f]+).*"
    "(0x[0-9A-Fa-f]+).*(0x[0-9A-Fa-f]+)");
    QRegExp rx2("(0x[0-9A-Fa-f]+).*(0x[0-9A-Fa-f]+)");
    //qDebug() << "Entered getFpRegs";
    bool reverse = ebe["xmm/reverse"].toBool();
    if (!running) return;
    results = sendReceive("info vector");
    foreach ( result, results ) {
        //result = result.replace(QRegExp("[{}]"),"");
        //qDebug() << result;
        result = result.replace(QRegExp("\\s+,"),"");
        //qDebug() << result;
        result = result.replace(QRegExp("\\s+\\{"),"{");
        //qDebug() << result;
        result = result.replace("0x","");
        //qDebug() << result;
        if ( result.indexOf("mxcsr") < 0 ) {
            parts = result.split(QRegExp("[{}]"));
            n = parts.length();
            //qDebug() << parts;
            if ( n < 2 ) continue;
            reg = parts[0];
            value = "";
            for ( i = 1; i < n; i++ ) {
                part = parts[i];
                if ( part.indexOf("int8") >= 0 ) {
                    part = parts[i+1];
                    parts = part.split(", ");
                    //qDebug() << reg << parts;
                    n2 = parts.length();
                    if ( reverse ) {
                        for ( j = 0; j < n2; j += 8 ) {
                            s="0x";
                            for ( k = j; k < j + 8; k++ ) {
                                t = parts[k];
                                if (t.length() == 1 ) t = "0" + t;
                                s += t;
                            }
                            //qDebug() << s;
                            if ( j > 0 ) value += " ";
                            value += s;
                        }
                    } else {
                        for ( j = 0; j < n2; j += 8 ) {
                            s="0x";
                            for ( k = j+7; k >= j; k-- ) {
                                t = parts[k];
                                if (t.length() == 1 ) t = "0" + t;
                                s += t;
                            }
                            //qDebug() << s;
                            if ( j > 0 ) value += " ";
                            value += s;
                        }

                    }
                    data += value;
                    break;
                }
            }
        }
    }
    //qDebug() << "getFpRegs" << data;
    if ( data.size() == 0 ) running = false;
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

//void GDB::getLocals()
//{
    //QStringList locals;
    //QStringList results;
    //QStringList parts;
    //VariableDefinitionMap vars;
//
    //qDebug() << "getLocals";
    //if (!running) return;
    //results = sendReceive(QString("info locals"));
//
    //foreach ( QString r, results ) {
        //int n = r.indexOf(" =");
        //if ( n > 0 && r.at(0) != ' ' && r.at(0) != '_' ) {
            //locals.append(r.left(n));
        //}
    //}
    //locals.sort();
    //qDebug() << "locals" << locals;
//
    //getVars(locals, vars);
//
    //emit sendLocals(0,vars);
//}

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
    d = false;
    //qDebug() << "getVars" << names;
    foreach ( QString name, names ) {
        fortran = false;
        var.size = 0;
        if ( &names == &globals && name == "stack" ) {
            var.name = name;
            var.type = "unsigned long";
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
            //var.isFortran = fortran;
            foreach ( QString r, results ) {
                //qDebug() << "r" << r;
                r = r.replace(" \"","");
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
                QString d1 = parts[1];
                d1.chop(1);
                d1 = d1.mid(1);
                //qDebug() << var.name << d;
                parts = d1.split(",");
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
        //qDebug() << "dimensions" << var.dimensions.length();

        if ( var.type != "" ) {
            if ( &names == &globals && var.name == "stack" ) {
                //qDebug() << "stack";
                var.values = sendReceive("x/64xb $rsp");
            } else if ( simpleTypes.contains(var.type) ) {
                var.size = sizeForType[var.type];
                QString cmd = QString("x/%1xb &%2")
                    .arg(var.size).arg(name);
                //qDebug() << "simple type" << var.type << cmd;
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
                //qDebug() << "std::string" << cmd;
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
                //if ( isFortran ) {
                    //var.size = sizeForType[baseType]*(n2-n1+1);
                //} else {
                    n3 = t.indexOf("[");
                    t = t.left(n3).trimmed();
                    var.size = sizeForType[t]*(n2-n1+1);
                //}
                QString cmd = QString("x/%1xb %2")
                    .arg(var.size).arg(name);
                //qDebug() << "array" << cmd;
                var.values = sendReceive(cmd);
                //qDebug() << var.values;
                var.value = "";
            } else if ( var.type == "char **" ) {
                var.values.clear();
                int i = 0;
                //qDebug() << "char **" << var.name;
                while ( 1 ) {
                    cmd = QString("x/s %1[%2]").arg(name).arg(i);
                    results = sendReceive(cmd);
                    if ( results.length() == 0 ) break;
                    if ( results[0].indexOf("Cannot access memory") > 0 ) break;
                    parts = results[0].split(QRegExp(":\\s+"));
                    if ( parts.length() < 2 ) break;
                    if ( parts[0] == "0x0" ) break;
                    var.values += parts[1];
                    i++;
                }
                //qDebug() << var.name << var.values;
            } else if ( var.type.indexOf("*") >= 0 ) {
                //qDebug() << "pointer" << var.name;
                var.size = wordSize/8;
                QString cmd = QString("x/%1xb &%2").arg(var.size).arg(name);
                var.values = sendReceive(cmd);
            } else {
                results = sendReceive(QString("print sizeof(%1)").arg(name));
                //qDebug() << "default:" << results;
                if ( results.length() >= 1 ) {
                    parts = results[0].split(QRegExp("[ =]+"));
                    //qDebug() << "parts" << parts;
                    if ( parts.length() >= 2 ) {
                        var.size = parts[1].toInt();
                    } else {
                        var.size = 8;
                    }
                } else {
                    var.size = 8;
                }
                QString cmd = QString("x/%1xb &%2").arg(var.size).arg(name);
                var.values = sendReceive(cmd);
            }
            vars[var.name] = var;
        }
    }
    d = false;
}

void GDB::getArgs()
{
    QStringList args;
    QStringList results;
    QStringList parts;
    VariableDefinitionMap vars;

    if (!running) return;
    acquireSem(gateSem);
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
    releaseSem(gateSem);
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
    acquireSem(gateSem);
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
    releaseSem(gateSem);
}

void GDB::requestLocals(DataPlank *p, int frame)
{
    QStringList locals;
    QStringList results;
    QStringList parts;
    VariableDefinitionMap vars;

    if (!running) return;
    acquireSem(gateSem);
    d = false;
    send(QString("frame %1").arg(frame));
    //qDebug() << "requestLocals" << QString("frame %1").arg(frame);
    results = sendReceive(QString("info locals"));
    //qDebug() << results;
    foreach ( QString r, results ) {
        int n = r.indexOf(" =");
        if ( n > 0 && r.at(0) != ' ' && r.at(0) != '_' ) {
            locals.append(r.left(n));
        }
    }
    //locals.sort();
    //qDebug() << "requestLocals: locals:" << locals;
    getVars(locals, vars);
    emit sendLocals(p,vars);

    send("frame 0");
    d = false;
    releaseSem(gateSem);
}

void GDB::requestVar(DataPlank *p, QString name, QString address,
        QString /*type*/, QString format, int size, int frame)
{
    QStringList results;
    QStringList temp;
    QStringList parts;
    QString result;
    QString cmd;
    QString address2;
    //int first = 0;
    //int last = 0;

    //qDebug() << "gdb requestVar" << name << address << format << size << frame;
    if (!running) return;
    acquireSem(gateSem);

    send(QString("frame %1").arg(frame));
    if (format == "string array") {
        int i = 0;
        while (1) {
            cmd = QString("x/s ((char **)%1)[%2]").arg(name).arg(i);
            //qDebug() << cmd;
            temp = sendReceive(cmd);
            //qDebug() << temp;
            if (temp.length() == 0) break;
            parts = temp[0].split(QRegExp(":\\s+"));
            if (parts.length() < 2) break;
            if ( parts[0] == "0x0" ) break;
            results += parts[1];
            i++;
        }
        //qDebug() << "string array" << results;
    } else if (format == "std::string") {
        cmd = QString("x/s %1.c_str()").arg(address);
        //qDebug() << cmd;
        temp = sendReceive(cmd);
        result = "";
        if ( temp.length() > 0 ) {
            parts = temp[0].split(QRegExp(":\\s+"));
            if (parts.length() >= 2 && parts[0] != "0x0" ) {
                result = parts[1];
                results += parts[1];
            }
        }
    } else if (format == "string") {
        cmd = QString("x/s (char *)%1").arg(address);
        results = sendReceive(cmd);
        if (results.length() == 0) {
            result = "";
        } else {
            int n = results[0].indexOf("\"");
            result = results[0].mid(n);
        }
    } else if (format == "pointer") {
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
        results = sendReceive(cmd);
        //qDebug() << "cmd" << cmd;
        //qDebug() << results;
    }
    send("frame 0");
    emit sendVar(p, name, results);
    releaseSem(gateSem);
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
    if (format == "hexadecimal") formatLetter = 'x';
    else if (format == "floating point") formatLetter = 'f';
    else if (format == "character") formatLetter = 'c';
    else if (format == "string") formatLetter = 's';

    if (first < 0 || last < 0) return;

    acquireSem(gateSem);
    if (format == "string array") {
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
    } else if (format == "pointer") {
        cmd = QString("printf \"0x%x\\n\",%1").arg(name);
        results = sendReceive(cmd);
        if (results.length() == 0) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            result = parts[parts.length() - 1];
        }
    } else if (format == "string") {
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
    releaseSem(gateSem);
}

void GDB::setNormal()
{
    if (!running) return;
    //send("set language "+currentLanguage);
    if (NullEOF) {
        send("call __ebeSetNormal()");
        NullEOF = false;
    }
    emit endFlash();
}

void GDB::setEOF()
{
    if (!running) return;
    acquireSem(gateSem);
    send("call __ebeSetNULL()");
    NullEOF = true;
    releaseSem(gateSem);
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
    acquireSem(gateSem);
    if ( wordSize == 64 ) {
        results = sendReceive(QString("x/%1xg $rsp").arg(n));
    } else {
        results = sendReceive(QString("x/%1xw $esp").arg(n));
    }
    //qDebug() << "requestStack" << results;
    emit receiveStack(results);
    releaseSem(gateSem);
}

void GDB::requestAsmVariable(int i, uLong address, int n)
{
    QStringList results, list;
    if (!running) return;
    acquireSem(gateSem);
    results = sendReceive(QString("x/%1xb %2").arg(n).arg(address));
    foreach (QString s, results) {
        s = s.replace(QRegExp(" <.*>"),"");
        list += s;
    }
    //qDebug() << "requestAsmVariable" << list;
    emit sendAsmVariable(i,list);
    releaseSem(gateSem);
}
