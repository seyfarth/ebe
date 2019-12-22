#include <QDebug>
#include <QTimer>
#include <cstdio>
#include "lldb.h"
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

extern ClassDefinition latestClass;

extern Debugger *dbg;

extern LLDBThread *lldbThread;

static volatile bool blocked;
static volatile bool resultReady;
extern bool d;

static QSemaphore gateSem(1);

LLDB *lldb;

LLDBThread::LLDBThread()
    : QThread()
{
}

void LLDBThread::run()
{
    //qDebug() << "lldb thread" << lldbThread;
    dbg = lldb = new LLDB;
    dbg->initDBG();
    Debugger::runningStatus = Debugger::dbgProcess->state();
    exec();
}

LLDBReaderThread::LLDBReaderThread()
    : QThread()
{
}

void LLDBReaderThread::run()
{
    //qDebug() << "reader thread" << QThread::currentThread();
    //qDebug() << "in run reader state" << dbg->dbgProcess->state() << dbg->dbgProcess->pid();
    exec();
}

void LLDBReaderThread::handleRead()
{
    //qDebug() << "reader HR thread" << QThread::currentThreadId();
    lldb->handleRead();
}


LLDB::LLDB()
    : Debugger()
{
#if defined(Q_OS_WIN)
    needToKill = false;
#endif
    dbgProcess = new QProcess();
    dbgProcess->start(dbgName,QIODevice::ReadWrite | QIODevice::Text );
    dbgProcess->setProcessChannelMode(QProcess::MergedChannels);
    //dbgProcess->start(dbgName);

    wordSize = ebe["build/word_size"].toInt();
    //qDebug() << "lldb state" << dbgProcess->state() << dbgProcess->pid();
    //sleep(2);
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
    blocked = true;
    //reader = new LLDBReaderThread();
    //reader->moveToThread(reader);
    this->moveToThread(lldbThread);
    //qDebug() << "lldbThread" << lldbThread;
    //connect ( lldb->dbgProcess, SIGNAL(readyReadStandardOutput()),
              //reader, SLOT(handleRead()) );
    reader->start();
    //qDebug() << "reader" << reader;
    QFile::copy(":/src/wrapper.py", "wrapper.py");
    sendRaw("command script import wrapper.py");
}

void LLDB::sync()
{
    while ( blocked ) {
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        usleep(1000);
    }
}

void LLDB::waitForResults()
{
    while ( !resultReady ) {
        QCoreApplication::processEvents(QEventLoop::AllEvents,1);
    }
}

QString cmdTag;

void LLDB::sendRaw(QString cmd, QString tag)
{
    //qDebug() << "sendRaw" << cmd;
    cmdTag = tag;
    writeLine(cmd);
}

void LLDB::send(QString cmd, QString tag)
{
    sendReceive(cmd,tag);
}

static bool firstRead = true;
static QStringList hrResults;

QStringList LLDB::sendReceive(QString cmd, QString tag)
{
    QString s;
    QString result;
    QStringList results;
    bool stopped;

    cmd = "wrap " + cmd;
    //qDebug() << "sendReceive start:"<< cmd << tag;
    //qDebug() << "sendReceive thread" << QThread::currentThread();
    cmdTag = tag;
    writeLine(cmd);
    emit log(cmd);
    blocked = true;
    stopped = false;
    while ( 1 ) {
        //qDebug() << "bytesAvailable" << dbgProcess->bytesAvailable();
        if ( dbgProcess->bytesAvailable() < 1 ) dbgProcess->waitForReadyRead(-1);
        result = dbgProcess->readLine();
        result.chop(1);
        //qDebug() << "sr read" << result;
        if ( result.indexOf("stopped") >= 0 ) stopped = true;
        //qDebug() << "stopped" << stopped  << result;
        if ( result == "LLDB DONE" ) break;
        if ( result.indexOf("(lldb)") < 0 ) {
            results.append(result);
        }            
    }
    //qDebug() << "sr" << results;
    if ( stopped && ((results.indexOf(QRegExp("Target.*")) >= 0)) ) {
        int k;
        k = results.indexOf(QRegExp(".* at [^:]+:[0-9]+:[0-9]+"));
        //qDebug() << "look for file" << k;
        if ( k >= 0 ) {
            handleNextInstruction(results[k]);
        } else {
	    k = results.indexOf(QRegExp("->.*"));
            if ( k >= 0 ) handleNextInstruction(results[k]);
        }
    } else if ( stopped && results.indexOf(QRegExp(".*exited.*")) >= 0 ) {
        //qDebug() << "exited bt" << result;
        results.append(tr("Program not running"));
        running = false;
        emit sendBackTrace(results);
    }
    //qDebug() << "SR results" << results;
    foreach ( s, results ) emit log(s);

    return results;
}

QStringList LLDB::splitLines(QString s)
{
    s = s.replace("\"","");
    s = s.replace(QRegExp("\\\\n$"),"");
    return s.split("\\n");
}

void LLDB::handleRead()
{
    QString result;
    QStringList results;
    bool stopped;
    QStringList parts;

    //qDebug() << "Entered handleRead" << cmdTag;
    //qDebug() << "handleRead thread" << QThread::currentThreadId();
    if ( firstRead ) {
        firstRead = false;
        blocked = false;
        //return;
    }

    stopped = false;
    //while ( dbgProcess->bytesAvailable() > 0 ) {
    while ( 1 ) {
        if ( dbgProcess->bytesAvailable() > 0 ) {
            //qDebug() << "bytesAvailable" << dbgProcess->bytesAvailable();
            result = dbgProcess->readLine();
            result.chop(1);
            emit log(result);
            //qDebug() << "hr" << result;
            if ( result.indexOf("stopped") >= 0 ) stopped = true;
            //qDebug() << "stopped" << stopped  << result;
            if ( result.indexOf("prompt (string)") == 0 ) break;
            if ( result.indexOf("(lldb)") < 0 ) {
                results.append(result);
            }            
        } else {
            lldbThread->wait(100);
            usleep(1000);
            //QCoreApplication::processEvents(QEventLoop::AllEvents);
            //qDebug() << "after sleep bytesAvailable" << dbgProcess->bytesAvailable();
        }
    }
    hrResults = results;
    //qDebug() << "handleRead" << results;
    if ( stopped && ((result.indexOf("Target") >= 0) ||
          (results.indexOf("step") >= 0)) ) {
        int k;
        k = results.indexOf(QRegExp("->.*"));
        //qDebug() << "stopped" << result << k;
        if ( k >= 0 ) handleNextInstruction(results[k]);
    } else if ( stopped && result.indexOf("exited") >= 0 ) {
        //qDebug() << "exited bt" << result;
        results.append(tr("Program not running"));
        running = false;
        emit sendBackTrace(results);
    } else if ( result.indexOf("(lldb) set show") ) {
        blocked = false;
        resultReady = true;
    }
    //qDebug() << "exit handleRead";
}

void LLDB::handleNextInstruction(QString result)
{
    QStringList parts;
    QString file;
    int line;
    int n;
    unsigned long address=0;

    parts = result.split(QRegExp(" +"));
    //qDebug() << "stopped" << parts;
    n = parts.length();
    if ( n <= 2 ) return;
    if ( parts[0] == "->" ) {
        address = parts[1].toULong(0,16);
    } else {
        QStringList parts2;
        parts2 = parts[n-1].split(':');
        if ( parts2.length() == 3 ) {
            file = parts2[0];
            line = parts2[1].toInt();
        } else {
            return;
        }
    }
    if ( address != 0 ) {
        FileLine fl;
        if ( addressToFileLine.contains(address) ) {
            fl = addressToFileLine[address];
            //qDebug() << address << fl.file << fl.line;
            emit nextInstruction(fl.file,fl.line);
        } else {
            //qDebug() << tr("Could not interpret address:") << address;
        }
        //qDebug() << "stopped" << address << file << line;
    } else {
        emit nextInstruction(file,line);
        //qDebug() << "stopped" << address << file << line;
    }
}

void LLDB::completeStep()
{
    acquireSem(gateSem);
    //qDebug() << "entered completeStep, running" << running;
    if ( running ) setNormal();
    //qDebug() << "after setNormal";
    if ( running ) getClasses();
    //qDebug() << "after getClasses";
    if ( running ) getRegs();
    //qDebug() << "after getRegs";
    if ( running ) getFpRegs();
    //qDebug() << "after getFpRegs";
    if ( running ) getBackTrace();
    //qDebug() << "after getBackTrace";
    if ( running ) getGlobals();
    //qDebug() << "after getGlobals";
    //qDebug() << "completed step";
    if ( running ) emit resetData();
    releaseSem(gateSem);
}

void LLDB::processTypedefRequest(QString name, QString &type)
{
    VariableDefinition v;
    QString result;
    QStringList results;
    QStringList parts;
    QRegExp rx("^\\s+([a-zA-Z].*)\\s+(\\**)([a-zA-Z0-9][a-zA-Z0-9_]*)(.*);$");
    acquireSem(gateSem);
    //qDebug() << "processTypedefRequest" << name;
    latestClass.name = name;
    latestClass.members.clear();
    results = sendReceive("type lookup " + name);
    foreach(result, results)
    {
        //qDebug() << result;
        type = result;
    }
    releaseSem(gateSem);
}

void LLDB::processClassRequest(QString name, ClassDefinition &c)
{
    QRegExp rx("^\\s+([a-zA-Z].*)\\s+(\\**)([a-zA-Z0-9][a-zA-Z0-9_]*)(.*);$");
    QString result;
    QStringList results;
    VariableDefinition v;
    acquireSem(gateSem);
    c.name = name;
    c.members.clear();
    //results = sendReceive("ptype " + name);
    foreach(result, results)
    {
        if (result.indexOf("    ") >= 0 && result.indexOf('(') < 0) {
            //qDebugu) << result;
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

void LLDB::getClasses()
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

    d = true;
    results = sendReceive("bt");
    //qDebug() << "getClasses" << results;
    n = results.length();
    if ( n < 2 ) {
        d = false;
        return;
    }
    if ( results[0].indexOf("thread") >= 0 ) {
        results.removeFirst();
        n--;
    }

    frame = 0;
    foreach ( result, results ) {
        if ( result.indexOf("libc_start_main") >= 0 ) {
            return;
        }
        if ( result.indexOf(" at ") < 0 ) {
            frame++;
            continue;
        }
        send(QString("frame select %1").arg(frame));
        if ( frame == 0 ) res2 = sendReceive("frame variable -g");
        else res2 = sendReceive("frame variable");
        //qDebug() << "frame" << frame << res2;
        foreach ( s, res2 ) {
            parts = s.split(QRegExp("[()]"));
            //qDebug() << "parts" << parts;
            if ( parts[0] != "" ) continue;
            t = parts[1];
            QString firstT = t;
            while ( 1 ) {
                t = t.replace("volatile ","");
                t = t.replace("const ","");
                t = t.replace(" const","");
                t = t.replace("static ","");
                t = t.replace(QRegExp(" \\*+"),"");
                t = t.replace(QRegExp("\\[.*"),"");
                //qDebug() << "type" << t;
                if ( simpleTypes.contains(t) ) break;
                res3 = sendReceive("type lookup " + t );
                //qDebug() << "type" << t << res3;
                if ( res3.length() == 1 ) {
                    t = res3[0];
                    if ( t == firstT ) break;
                } else {
                    c.name = t;
                    c.members.clear();
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
                    foreach ( r, res3 ) {
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
                    foreach ( v, c.members ) {
                        //qDebug() << "   " << t << v.name << v.type;
                    }
                    classes[t] = c;
                    break;
                }
            }
        }
        if ( result.indexOf("`main") >= 0 ) break;
        frame++;
    }
        
    //qDebug() << "getClasses new types" << newTypes;

    while ( !newTypes.empty() ) {
        t = newTypes.first();
        //qDebug() << "first" << t;
        newTypes.removeFirst();
        if ( classes.count(t) > 0 ) continue;
        c.name = t;
        c.members.clear();
        results = sendReceive("type lookup " + t );
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
            results = sendReceive("type lookup " + t );
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

    emit sendClasses(classes);
    d = false;
}

void LLDB::initDBG()
{
    //qDebug() << "initDBG entered";
    running = false;
    dbgProcess->setTextModeEnabled(true);
    dbgProcess->setReadChannel(QProcess::StandardOutput);
    send("set set term-width 1024");
    //qDebug() << "initDBG exiting";
}

//public slots:
void LLDB::doCommand(QString cmd, QString tag)
{
    acquireSem(gateSem);
    if ( tag == "" ) tag = "console";
    //qDebug() << "doCommand" << cmd << tag << p1 << p2;
    send(cmd,tag);
    releaseSem(gateSem);
}

QString currentFile;

void LLDB::doRun(QString exe, QString options, QStringList files,
    QList<StringSet> breakpoints, QStringList g)
{
    int i;
    int length = files.length();
    QStringList parts;
    QString inputFile;
    QString outputFile;
    QString errorFile;
    QString newOptions;
    //isFortran = false;
    globals = g;
    globals.append("stack");
    //qDebug() << "doRun" << exe << globals;
    //qDebug() << "doRun" << exe << "length" << length;
    running = false;
    bpHash.clear();
    //send("set set prompt \"(lldb)\\n\"");
    runningStatus = dbgProcess->state();
    if ( runningStatus != QProcess::Running ) {
        emit error("lldb is not running");
        return;
    }
    acquireSem(gateSem);
    send("kill");
    //send("nosharedlibrary");
    send(QString("script os.chdir('") + QDir::currentPath() + QString("')"));
    //qDebug() << "file" << exe;
    if ( exe != currentFile ) {
        send("file \"" + exe + "\"");
        currentFile = exe;
    }

    if ( options.length() > 0 ) {
        parts = options.split(QRegExp("\\s+"));
        int n;
        n = parts.length();
        for ( i = 0; i < n; i++ ) {
            if ( i < n-1 && parts[i] == "<" ) {
                inputFile = parts[i+1];
                i++;
            } else if ( i < n-1 && parts[i] == ">" ) {
                outputFile = parts[i+1];
                i++;
            } else if ( i < n-1 && parts[i] == "2>" ) {
                errorFile = parts[i+1];
                i++;
            } else {
                newOptions += parts[i];
                if ( i < n-1 ) newOptions += " ";
            }
        }
    }
    options = newOptions;

    //qDebug() << "set opts" << options;
    if ( options.length() > 0 ) {
        send("set set target.run-args " + options);
    } else {
        send("set clear target.run-args");
    }
    QString terminalName = terminalWindow->ptyName;
    if ( ebe["terminal/use"].toBool() ) {
        terminalName = ebe["terminal/name"].toString();
    }
    if ( inputFile == "" ) {
        send("set set target.input-path " + terminalName );
    } else {
        send("set set target.input-path " + inputFile );
    }
    if ( outputFile == "" ) {
        send("set set target.output-path " + terminalName );
    } else {
        send("set set target.output-path " + outputFile );
    }
    if ( errorFile == "" ) {
        send("set set target.error-path " + terminalName );
    } else {
        send("set set target.error-path " + errorFile );
    }
    //send("process launch --no-stdio --stop-at-entry" );
    send("process launch --stop-at-entry", "launch" );
    send("break delete -f");
    for (i = 0; i < length; i++) {
        foreach ( QString bp, breakpoints[i] ) {
            //qDebug() << files[i] << bp;

            setBreakpointInternal(files[i],bp);
        }
    }
#if defined(Q_OS_WIN)
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
    running = true;
    hasAVX = testAVX();
    send("continue");
    //send("set prompt (lldb)\\n");
#else
    //qDebug() << "tty " << terminalWindow->ptyName;
    //send("set inferior-tty " + terminalWindow->ptyName);
    running = true;
    //sendReceive("2 register read ymm16");
    //qDebug() << "Before sleep";
    //usleep(1000);
    //qDebug() << "After sleep";
    hasAVX = testAVX();
    send("continue","run");
#endif
    //qDebug() << "running" << running;
    if ( running ) emit resetData();
    releaseSem(gateSem);
    //qDebug() << "released";
    //if (!running) return;
    //getClasses();
    //if (!running) return;
    //qDebug() << "Done run";

}

void LLDB::doNext()
{
    //qDebug() << "lldb next";
    if (!running) return;
    acquireSem(gateSem);
    send("next");
    //qDebug() << "end lldb next";
    releaseSem(gateSem);
}

void LLDB::doStep()
{
    //qDebug() << "gdb step";
    if (!running) return;
    acquireSem(gateSem);
    send("step");
    releaseSem(gateSem);
}

void LLDB::doNextInstruction()
{
    if (!running) return;
    acquireSem(gateSem);
    send("thread step-inst");
    releaseSem(gateSem);
}

void LLDB::doStepInstruction()
{
    //qDebug() << "stepi";
    if (!running) return;
    acquireSem(gateSem);
    send("stepi");
    releaseSem(gateSem);
}

void LLDB::doCall()
{
    FileLine fl(breakFile, breakLine + 1);
    //qDebug() << "call from" << breakFile << breakLine;
    //for ( int i = 1; i < 100; i++ ) {
        //FileLine fl2(breakFile, i );
        //qDebug() << "fl2" << i << fileLineToAddress[fl2];
    //}
    if (!running) return;
    acquireSem(gateSem);
    if ( ebe["build/assembler"].toString() == "nasm" ) {
        //qDebug() << "tbreak *" << fileLineToAddress[fl] << "OK";
        send(QString("tbreak -a %1").arg(fileLineToAddress[fl]));
//#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
        //send(QString("tbreak *%1").arg(fileLineToAddress[fl]));
//#else
    } else {
        send(QString("tbreak -f \"%1\" -l %2").arg(breakFile).arg(breakLine+1));
    }
//#endif
    send("continue","call");
    releaseSem(gateSem);
}

void LLDB::doContinue()
{
    //qDebug() << "continue";
    if (!running) return;
    acquireSem(gateSem);
    send("continue");
    releaseSem(gateSem);
}

void LLDB::doStop()
{
    //qDebug() << "doStop";
    if ( !running ) return;
    acquireSem(gateSem);
    send("kill");
    running = false;
    //qDebug() << "end doStop";
    releaseSem(gateSem);
}

void LLDB::setBreakpoint(QString file, QString bp)
{
    acquireSem(gateSem);
    setBreakpointInternal(file,bp);
    releaseSem(gateSem);
}

void LLDB::setBreakpointInternal(QString file, QString bp)
{
    QStringList results;
    QStringList parts;
    FileLabel f(file, bp);
    if (bpHash.contains(f)) {
        //qDebug() << "bp" << bp << "exists";
        return;
    }
    //send("set set prompt \"(lldb)\\n\"");
    if (bp[0] == QChar('*')) {
        //qDebug() << "setting" << bp;
        results = sendReceive(QString("b -a %1").arg(bp.remove(0,1)));
    } else {
        //qDebug() << QString("break \"%1\":%2").arg(file).arg(bp);
        results = sendReceive(QString("b -f %1 -l %2").arg(file).arg(bp));
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

void LLDB::requestReset()
{
    //if ( running ) emit resetData();
}

void LLDB::deleteBreakpoint(QString file, QString line)
{
    FileLabel f(file, line);
    int bp = bpHash[f];
    //qDebug() << "dbp" << file << line << bp;
    if (bp > 0) {
        acquireSem(gateSem);
        send(QString("break delete %1").arg(bp));
        bpHash.remove(f);
        releaseSem(gateSem);
    }
}

void LLDB::getBackTrace()
{
    QStringList results;
    int i,  n;

    if (!running) {
        results.clear();
        results.append(tr("Program not running"));
        emit sendBackTrace(results);
        return;
    }

    if ( !running ) return;
    results = sendReceive("bt");
    //qDebug() << "getBackTrace" << results;
    if (results.length() < 1 || results[0].indexOf("error:") >= 0 ) {
        running = false;
        results.clear();
        results.append(tr("Program not running"));
        emit sendBackTrace(results);
        return;
    }
    if ( results[0].indexOf("thread #") >= 0 ) results.removeFirst();
    n = results.length();

    for (i = 0; i < n; i++) {
        if (results[i].indexOf("`main") >= 0) {
            break;
        }
    }
    for (i++; i < n; i++) {
        results.removeLast();
    }
    if ( results.length() < 1 || results[0].indexOf("libc_start_main") >= 0 ) {
        results.clear();
        results.append(tr("Program not running"));
        send("kill");
        emit sendBackTrace(results);
        return;
    }
    //qDebug() << "getBackTrace" << results;
    n = results.length();
    for ( i=0; i < n; i++ ) {
        //qDebug() << results[i];
        results[i] = results[i].replace(QRegExp("[ *]+frame "),"");
        //qDebug() << results[i];
        results[i] = results[i].replace(": "," ");
        //qDebug() << results[i];
    }
    //qDebug() << "end gbt" << results;
    emit sendBackTrace(results);
}

void LLDB::getRegs()
{
    QStringList results;
    QStringList pp;
    StringHash map;
    QString s;

    if ( !running ) return;
    //d = true;
    send("frame select 0");
    results = sendReceive("register read");

    foreach ( QString part, results ) {
        pp = part.split(QRegExp("\\s+"));
        //qDebug() << pp;
        if ( pp.length() >= 4 ) {
            if ( regs.contains(pp[1]) ) {
                map[pp[1]] = pp[3];
            }
            if ( pp[1] == "rip" || pp[1] == "eip" ) {
                map[pp[1]] = pp[3];
            }
            if ( pp[1] == "rflags" || pp[1] == "eflags" ) {
                //qDebug() << "eflags" << pp;
                bool ok;
                int regdata = pp[3].toInt(&ok,16);
                for ( int i= 0; i < 7; i++ ) {
                    if ( regdata & reg_masks[i] ) s += reg_names[i] + " ";
                }
                //qDebug() << "eflags" << s;
                map["eflags"] = s;
            }
        }
    }
    //if(d) qDebug() << map;
    //d = false;
    emit sendRegs(map);
}

void LLDB::getFpRegs()
{
    QStringList results;
    QStringList parts;
    QStringList data;
    QString result;
    int i, j, n;
    QString value;
    QString fp;
    QString cmd;

    if ( !running ) return;

    cmd = "register read";
    foreach ( fp, fpRegs ) cmd += " " + fp;
    results = sendReceive(cmd);
    //qDebug() << "getFpRegs" << cmd;
    //bool reverse = ebe["xmm/reverse"].toBool();
    //qDebug() << "hfp" << result;
    
    //qDebug() << "hfp" << results;

    foreach ( result, results ) {
        result = result.replace("0x","");
        result = result.replace(QRegExp("[,{}()]"),"");
        result = result.replace(",","");
        parts = result.split(QRegExp("[\\s{}]+"));

        //qDebug() << "hfp " << result;
        //qDebug() << "parts" << parts;
        n = parts.length();
        if ( (n != 35) && (n != 19) ) continue;
        value = "";
        for ( i = 3; i < n;  i += 8 ) {
            if ( i > 3 ) value += " ";
            value += "0x";
            for ( j = 7; j >= 0; j-- ) {
                value += parts[i+j];
            }
        }
        data << value;
        //qDebug() << value;
    }

    //qDebug() << "hfp result" << data;
    if ( data.size() == 0 ) running = false;
    emit sendFpRegs(data);
}

void LLDB::getGlobals()
{
    QStringList results;
    VariableDefinitionMap vars;
    VariableDefinition var;

    //qDebug() << "getGlobals" << globals;
    if (!running) return;
    send("frame select 0");
    results = sendReceive("frame variable -L -g -a -l");
    var.name = "stack";
    var.type = "unsigned long *";
    var.value = "";
    var.size = 64;
    var.values = sendReceive("x/64xb $rsp");
    vars[var.name] = var;
    processVars(results, vars);
    emit sendGlobals(vars);
    //qDebug() << "end getGlobals";
}

//void LLDB::getLocals()
//{
    //QStringList locals;
    //QStringList results;
    //QStringList parts;
    //VariableDefinitionMap vars;

    //if (!running) return;
    //results = sendReceive("frame variable -a");
    //qDebug() << "getLocals" << results;
    //foreach ( QString r, results ) {
        //int n = r.indexOf(" =");
        ////if ( n > 0 && r.at(0) != ' ' && r.at(0) != '_' ) {
            ////locals.append(r.left(n));
        ////}
    //}
    //locals.sort();
//
    //getVars(locals, vars);
//
    //emit sendLocals(0,vars);
//}

void LLDB::getVars(QStringList &, VariableDefinitionMap &)
{
}

void LLDB::processVars(QStringList &results, VariableDefinitionMap &vars)
{
    QString result;
    QStringList parts;
    QStringList res2;
    QString type;
    QString address;
    Limits dim;
    VariableDefinition var;
    QString baseType;

    if (!running) return;

    foreach ( result, results ) {
        if ( result.indexOf("(") < 0 ) continue;
        result = result.replace(": ",":");
        result = result.replace("(","");
        result = result.replace(") ",")");
        result = result.replace(" = ","=");
        parts = result.split(QRegExp("[:)=]"));
        //qDebug() << "processVars" << parts;
        if ( parts.length() >= 4 ) {
            var.name = parts[2];
            var.value = parts[3];
            address = parts[0];
            type = parts[1];
            var.type = type;
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
            if ( simpleTypes.contains(var.type) ) {
                var.size = sizeForType[var.type];
            } else if ( var.dimensions.length() == 1 ) {
                int n1 = var.dimensions[0].first;
                int n2 = var.dimensions[0].last;
                QString t=var.type;
                int n3;
                n3 = t.indexOf("[");
                t = t.left(n3).trimmed();
                var.size = sizeForType[t]*(n2-n1+1);
            } else if ( var.type.indexOf("*") >= 0 ) {
                var.size = wordSize/8;
            } else {
                res2 = sendReceive(QString("e sizeof(%1)").arg(var.name));
                foreach ( QString r, res2 ) {
                    int i = r.indexOf("=");
                    if ( i >= 0 ) {
                        var.size = r.mid(i+2).toInt();
                        //qDebug() << var.name << "size" << var.size;
                        break;
                    }
                }
            }
        }
        //qDebug() << "processVars" << var.name << var.type << var.size << var.value;
        vars[var.name] = var;
    }
}

#if 0
        } else {
            var.name = name;
            var.type = "";
            var.value = "";
            results = sendReceive(QString("whatis %1").arg(name));
            //qDebug() << "results" << results;
            if ( results.length() > 0 &&
                    (results[0].indexOf("invalid") >= 0 ||
                     results[0].indexOf("(kind") >= 0 ) ) {
                //send ( "set language fortran");
                //results = sendReceive(QString("whatis %1").arg(name));
                //send ( "set language c++");
                fortran = true;
            }
            //var.isFortran = fortran;
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
        //qDebug() << "dimensions";

        if ( var.type != "" ) {
            if ( simpleTypes.contains(var.type) ) {
                var.size = sizeForType[var.type];
                QString cmd = QString("x/%1xb &%2")
                    .arg(var.size).arg(name);
                //qDebug() << cmd;
                //var.values = sendReceive(cmd);
                //qDebug() << var.values;
            } else if ( var.type.indexOf("character") == 0 ) {
                //qDebug() << name << var.type;
                int i = var.type.indexOf("*");
                if ( i < 0 ) break;
                QString t = var.type.mid(i+1);
                //qDebug() << name << var.type << t;
                int n = t.toInt();
                if ( n > 1000 ) {
                    cmd = "p _"+name;
                    //results = sendReceive(cmd);
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
                    //results = sendReceive(QString("x/%1xb &%2").arg(n).arg(name));
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
                //results = sendReceive(cmd);
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
                //qDebug() << cmd;
                //var.values = sendReceive(cmd);
                //qDebug() << var.values;
                var.value = "";
            } else if ( var.type == "char **" ) {
                var.values.clear();
                int i = 0;
                while ( 1 ) {
                    cmd = QString("x/s %1[%2]").arg(name).arg(i);
                    //qDebug() << "char **" << cmd;
                    results = sendReceive(cmd);
                    //qDebug() << "char **" << results;
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
                //var.values = sendReceive(cmd);
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
                //var.values = sendReceive(cmd);
            } else {
                var.value = "";
            }
            vars[var.name] = var;
        }
    }
}
#endif

void LLDB::getArgs()
{
    QStringList args;
    QStringList results;
    QStringList parts;
    VariableDefinitionMap vars;

    if (!running) return;
    //results = sendReceive(QString("info args"));
    foreach ( QString r, results ) {
        int n = r.indexOf(" =");
        if ( n > 0 && r.at(0) != ' ' && r.at(0) != '_' ) {
            args.append(r.left(n));
        }
    }
    args.sort();
    getVars(args, vars);
    //emit sendParameters(0,vars);
}

bool LLDB::testAVX()
{
    QStringList results;
    QStringList parts;
    bool avx = false;
    if (!running) return false;
    if ( fpRegs.count() > 0 ) return hasAVX;
    results = sendReceive("register read ymm0");
    foreach ( QString result, results ) {
        parts = result.split(QRegExp("\\s+"));
        //qDebug() << "testAVX" << parts;
        if ( parts.length() >= 3 && parts[2] == "=" ) avx = true;
    }
    //qDebug() << "avx" << avx;
    hasAVX = avx;
    if ( avx ) {
        if ( wordSize == 32 ) {
            fpRegs << "ymm0" << "ymm1" << "ymm2" << "ymm3"
                   << "ymm4" << "ymm5" << "ymm6" << "ymm7";
        } else {
            fpRegs << "ymm0"  << "ymm1"  << "ymm2"  << "ymm3"
                   << "ymm4"  << "ymm5"  << "ymm6"  << "ymm7"
                   << "ymm8"  << "ymm9"  << "ymm10" << "ymm11"
                   << "ymm12" << "ymm13" << "ymm14" << "ymm15";
        }
    } else {
        if ( wordSize == 32 ) {
            fpRegs << "xmm0" << "xmm1" << "xmm2" << "xmm3"
                   << "xmm4" << "xmm5" << "xmm6" << "xmm7";
        } else {
            fpRegs << "xmm0"  << "xmm1"  << "xmm2"  << "xmm3"
                   << "xmm4"  << "xmm5"  << "xmm6"  << "xmm7"
                   << "xmm8"  << "xmm9"  << "xmm10" << "xmm11"
                   << "xmm12" << "xmm13" << "xmm14" << "xmm15";
        }
    }
    //qDebug() << "fpRegs" << fpRegs;
    return avx;
}

void LLDB::requestParameters(DataPlank *p, int frame)
{
    QStringList args;
    QStringList results;
    QStringList parts;
    VariableDefinitionMap vars;

    if (!running) return;
    acquireSem(gateSem);
    send(QString("frame select %1").arg(frame));
    results = sendReceive("frame variable -L -l");
    //qDebug() << "requestParameters" << p << QString("frame %1").arg(frame);
    //qDebug() << results;
    processVars(results, vars);
    emit sendParameters(p,vars);

    send("frame select 0");
    releaseSem(gateSem);
}

void LLDB::requestLocals(DataPlank *p, int frame)
{
    QStringList locals;
    QStringList results;
    QStringList parts;
    VariableDefinitionMap vars;

    if (!running) return;

    acquireSem(gateSem);
    //qDebug() << "requestLocals" << p << frame;
    send(QString("frame select %1").arg(frame));
    results = sendReceive("frame variable -L -a");
    //qDebug() << "requestLocals: results:" << results;
    processVars(results, vars);
    emit sendLocals(p,vars);

    send("frame select 0");
    releaseSem(gateSem);
}

void LLDB::requestVar(DataPlank *p, QString name, QString address,
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

    //qDebug() << "lldb requestVar" << name << address << format << size << frame << p;
    if (!running) return;

    acquireSem(gateSem);
    send(QString("frame select %1").arg(frame));
    if (format == "string array") {
        int i = 0;
        while (1) {
            cmd = QString("x/s %1[%2]").arg(name).arg(i);
            //qDebug() << cmd;
            temp = sendReceive(cmd);
            //qDebug() << temp;
            if (temp.length() == 0) break;
            parts = temp[0].split(QRegExp(":\\s+"));
            if (parts.length() < 2) break;
            if ( parts[0] == "0x0" ) break;
            parts[1] = parts[1].replace("\\","");
            results += parts[1];
            i++;
        }
        //qDebug() << "string array" << results;
    } else if (format == "std::string") {
        cmd = QString("memory read -G s %1.c_str()").arg(address);
        //qDebug() << cmd;
        temp = sendReceive(cmd);
        result = "";
        if ( temp.length() > 0 ) {
            parts = temp[0].split(QRegExp(":\\s+"));
            if (parts.length() >= 2 && parts[0] != "0x0" ) {
                parts[1] = parts[1].replace("\\","");
                result = parts[1];
                results += parts[1];
            }
        }
    } else if (format == "string") {
        cmd = QString("memory read -G s (char *)%1").arg(address);
        //qDebug() << cmd;
        results = sendReceive(cmd);
        if (results.length() == 0) {
            result = "";
        } else {
            int n = results[0].indexOf("\"");
            result = results[0].mid(n);
        }
    } else if (format == "pointer") {
        cmd = QString("e printf(\"0x%x\\n\",%1)").arg(address);
        results = sendReceive(cmd);
        //qDebug() << cmd << results;
        if (results.length() == 0) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            result = parts[parts.length() - 1];
        }
    } else {
        cmd = QString("memory read -G %1xb %2").arg(size).arg(address);
        results = sendReceive(cmd);
        //qDebug() << "cmd" << cmd;
        //qDebug() << results;
    }
    send("frame select 0");
    emit sendVar(p, name, results);
    releaseSem(gateSem);
}

void LLDB::getData(QStringList request)
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
    acquireSem(gateSem);
    char sizeLetter = letterForSize[size];

    char formatLetter = 'd';
    if (format == "hexadecimal") formatLetter = 'x';
    else if (format == "floating point") formatLetter = 'f';
    else if (format == "character") formatLetter = 'c';
    else if (format == "string") formatLetter = 's';

    if (first < 0 || last < 0) return;

    if (format == "string array") {
        result = "";
        int i = 0;
        while (1) {
            cmd = QString("x/dg ((unsigned char *)%1)+%2").arg(address).arg(
                    i * (wordSize == 64 ? 8 : 4));
            //qDebug() << cmd;
            //results = sendReceive(cmd);
            //qDebug() << results;
            if (results.length() == 0) break;
            parts = results[0].split(QRegExp(":\\s+"));
            if (parts.length() < 2) break;
            address2 = parts[1];
            if (address2 == "0") break;
            cmd = QString("x/sb %1").arg(address2);
            //results = sendReceive(cmd);
            if (results.length() == 0) break;
            parts = results[0].split(QRegExp(":\\s+"));
            if (parts.length() < 2) break;
            result += parts[1] + " ";
            i++;
        }
    } else if (format == "pointer") {
        cmd = QString("printf \"0x%x\\n\",%1").arg(name);
        //results = sendReceive(cmd);
        if (results.length() == 0) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            result = parts[parts.length() - 1];
        }
    } else if (format == "string") {
        cmd = QString("x/s (char *)%1").arg(address);
        //qDebug() << cmd;
        //results = sendReceive(cmd);
        //qDebug() << results;
        if (results.length() == 0) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            result = parts[parts.length() - 1];
        }
    } else if (first == 0 && last == 0) {
        cmd = QString("memory read -G 1x%1 %2").arg(sizeLetter).arg(address);
        results = sendReceive(cmd);
        //qDebug() << "getData:" << cmd;
        //qDebug() << "results" << results;
        if (results.length() == 0) {
            result = "";
        } else {
            parts = results[0].split(QRegExp(":\\s+"));
            //qDebug() << "parts" << parts;
            if (parts.length() < 2) {
                result = "";
            } else if (parts[1] == "(lldb)") {
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
            //results = sendReceive(cmd);
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
    //emit dataReady(request);
    releaseSem(gateSem);
}

void LLDB::setNormal()
{
    //qDebug() << "setNormal" << running;
    if (!running) return;
    //send("language cplusplus");
    if (NullEOF) {
        send("call __ebeSetNormal()");
        NullEOF = false;
    }
    emit endFlash();
}

void LLDB::setEOF()
{
    if (!running) return;
    acquireSem(gateSem);
    send("call __ebeSetNULL()");
    NullEOF = true;
    releaseSem(gateSem);
}

void LLDB::receiveWorkingDir(QString dir)
{
    acquireSem(gateSem);
    QDir::current().cd(dir);
    releaseSem(gateSem);
}

void LLDB::requestStack(int n)
{
    QStringList results;
    //qDebug() << "requestStack" << QString("memory read -G %1xg $rsp").arg(n);
    if (!running) return;
    acquireSem(gateSem);
    if ( wordSize == 64 ) {
        results = sendReceive(QString("memory read -G %1xg $rsp").arg(n),"stack");
    } else {
        results = sendReceive(QString("memory read -G %1xw $esp").arg(n),"stack");
    }
    //qDebug() << results;
    emit receiveStack(results);
    releaseSem(gateSem);
}

void LLDB::requestAsmVariable(int i, uLong address, int n)
{
    QStringList results;
    if (!running) return;

    acquireSem(gateSem);
    //qDebug() << "requestAsmVariable" << i << address << n;

    results = sendReceive(QString("memory read -G %1xb %2").arg(n).arg(address));
    //qDebug() << "requestAsmVariable" << results;
    emit sendAsmVariable(i,results);
    releaseSem(gateSem);
}
