#include "sourceframe.h"
#include "projectwindow.h"
#include "commandline.h"
#include "stylesheet.h"
#include "errorwindow.h"
#include "librarywindow.h"
#include "instructions.h"
#include "gdb.h"
#include "settings.h"
#include "types.h"
#ifdef Q_WS_WIN
#include <windows.h>
#endif

#include <iostream>
using namespace std;

#ifdef Q_WS_WIN
extern HANDLE hProcess;
extern bool needToKill;
#endif

QStringList fortranExts;
QStringList cExts;
QStringList cppExts;
QStringList asmExts;
StringSet textLabels;

QHash<QString,Range> labelRange;
QHash<long,FileLine> addressToFileLine;
QMap<FileLine,unsigned long> fileLineToAddress;

StringHash varToAddress;
QSet<FileLine> callLines;

extern ProjectWindow *projectWindow;
extern GDB *gdb;
extern QStatusBar *statusBar;
extern SourceFrame *sourceFrame;

typedef QPair<QString,QString> StringPair;

SourceFrame::SourceFrame(QWidget *parent) : QFrame(parent)
{
    sourceFrame = this;
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    breakFile = "";
    breakLine = 0;

    bool icons = ebe["buttons/icons"].toBool();
    int icon_size = ebe["buttons/icon_size"].toInt();

    setStyleSheet("QPushButton { font-family: " +
            ebe["variable_font"].toString() + "}" +
            "QLabel { font-family:" +
            ebe["variable_font"].toString() + "}" );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(10,10,10,10);

    if ( ebe["buttons/visible"].toBool() ) {
        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->setSpacing(6);
        buttonLayout->setContentsMargins(2,2,2,2);

        quitButton     = new QPushButton;
        if ( icons ) {
            quitButton->setIcon(QIcon(QString(":/icons/%1/quit.png")
                        .arg(icon_size)));
            quitButton->setIconSize(QSize(icon_size,icon_size));
        } else {
            quitButton->setText("Quit");
        }
        quitButton->setStyleSheet ( "color: "+ebe["quit_color"].toString() );
        quitButton->setToolTip ( tr("Click this button to exit from ebe") );
        runButton      = new QPushButton;
        runButton->setToolTip ( tr("Compile and run your program") );
        runButton->setStyleSheet ( "color: "+ebe["run_color"].toString() );
        if ( icons ) {
            runButton->setIcon(QIcon(QString(":/icons/%1/run.png")
                        .arg(icon_size)));
            runButton->setIconSize(QSize(icon_size,icon_size));
        } else {
            runButton->setText("Run");
        }
        nextButton     = new QPushButton;
        nextButton->setToolTip (
                tr("Execute the current statement in the same function") );
        nextButton->setStyleSheet ( "color: "+ebe["next_color"].toString() );
        if ( icons ) {
            nextButton->setIcon(QIcon(QString(":/icons/%1/next.png")
                        .arg(icon_size)));
            nextButton->setIconSize(QSize(icon_size,icon_size));
        } else {
            nextButton->setText("Next");
        }
        stepButton     = new QPushButton;
        stepButton->setStyleSheet ( "color: "+ebe["step_color"].toString() );
        stepButton->setToolTip (
                tr("Execute the current statement possibly stepping\n"
                    "into a different function") );
        if ( icons ) {
            stepButton->setIcon(QIcon(QString(":/icons/%1/step.png")
                        .arg(icon_size)));
            stepButton->setIconSize(QSize(icon_size,icon_size));
        } else {
            stepButton->setText("Step");
        }
        continueButton = new QPushButton;
        continueButton->setStyleSheet ( "color: "+ebe["continue_color"].toString() );
        continueButton->setToolTip (
                tr("Execute statements starting at the current statement\n"
                    "continuing until the program ends or a breakpoint\n"
                    "is reached.") );
        if ( icons ) {
            continueButton->setIcon(QIcon(QString(":/icons/%1/continue.png")
                        .arg(icon_size)));
            continueButton->setIconSize(QSize(icon_size,icon_size));
        } else {
            continueButton->setText("Continue");
        }
        stopButton     = new QPushButton;
        stopButton->setStyleSheet ( "color: "+ebe["stop_color"].toString() );
        stopButton->setToolTip (
                tr("End this debugging session and continue editing") );
        if ( icons ) {
            stopButton->setIcon(QIcon(":/icons/32/process-stop.png"));
            stopButton->setIconSize(QSize(icon_size,icon_size));
        } else {
            stopButton->setText("Stop");
        }
        buttonLayout->addWidget ( quitButton );
        buttonLayout->addWidget ( runButton );
        buttonLayout->addWidget ( nextButton );
        buttonLayout->addWidget ( stepButton );
        buttonLayout->addWidget ( continueButton );
        buttonLayout->addWidget ( stopButton );
        buttonLayout->addStretch();

        connect ( quitButton, SIGNAL(clicked()), parent, SLOT(quit()) );
        connect ( runButton, SIGNAL(clicked()), this, SLOT(run()) );
        connect ( nextButton, SIGNAL(clicked()), this, SLOT(next()) );
        connect ( stepButton, SIGNAL(clicked()), this, SLOT(step()) );
        connect ( continueButton, SIGNAL(clicked()), this, SLOT(Continue()) );
        connect ( stopButton, SIGNAL(clicked()), this, SLOT(stop()) );

        layout->addLayout(buttonLayout);
    }

    connect ( this, SIGNAL(doRun(QString,QString,QStringList,QList<StringSet>,QStringList)),
            gdb, SLOT(doRun(QString,QString,QStringList,QList<StringSet>,QStringList)) );
    connect ( this, SIGNAL(doNext()), gdb, SLOT(doNext()) );
    connect ( this, SIGNAL(doStep()), gdb, SLOT(doStep()) );
    connect ( this, SIGNAL(doContinue()), gdb, SLOT(doContinue()) );
    connect ( this, SIGNAL(doStop()), gdb, SLOT(doStop()) );
    connect ( gdb, SIGNAL(nextInstruction(QString,int)),
            this, SLOT(nextInstruction(QString,int)) );

    commandLine = new CommandLine();
    commandLine->setToolTip (
            tr("This input field is for executing your program with\n"
                "command line parameters to be accepted in the argv\n"
                "array") );

    tab = new QTabWidget;

    layout->addWidget(commandLine);
    layout->addWidget(tab);

    connect ( tab, SIGNAL(currentChanged(int)), this, SLOT(changedTab(int)) );

    cursorPosition = new QLabel(this);
    statusBar->addPermanentWidget(cursorPosition);

    setLayout(layout);

    source = new SourceWindow;
    int index = tab->addTab(source,"unnamed");
    tab->setCurrentIndex(index);

    fortranExts << "f" << "F" << "for" << "FOR" << "f90" << "F90"
        << "f95" << "F95" << "ftn" << "FTN";
    cExts << "c" << "C";
    cppExts << "cpp" << "CPP" << "c++" << "C++" << "cc" << "CC";
    asmExts << "asm" << "ASM" << "s" << "S";
}

void SourceFrame::saveIfChanged(QString file)
{
    int index;
    int count;
    SourceWindow *source;

    count = tab->count();
    for ( index = 0; index < count; index++ ) {
        source = (SourceWindow *)tab->widget(index);
        if ( source->fileName == file ) {
            source->save();
            break;
        }
    }
}

void SourceFrame::run()
{
    int index;
    int length;
    QStringList globals;
    QStringList sourceFiles;
    QList<StringSet> breakpoints;
    StringSet bps;
    QList<StringPair> objectFiles;
    QString object;
    QString exeName;
    QString ext;
    QString name;
    QString base;
    QString cmd;

    addressToFileLine.clear();
    labelRange.clear();
    stop();
    breakLine = 0;
    breakFile = "";

    QFile::remove("ebe_unbuffer.cpp");
    QFile::copy(":/src/ebe_unbuffer.cpp","ebe_unbuffer.cpp");
    QFile::setPermissions("ebe_unbuffer.cpp",
           QFile::ReadOwner | QFile::WriteOwner);

#ifdef Q_WS_WIN
    if ( needToKill ) {
        //printf("Killing handle %d\n",hProcess);
        TerminateProcess(hProcess,0);
        needToKill = false;
    }
#endif
    //
    //  Determine all source files
    //
    sourceFiles << "ebe_unbuffer.cpp";

    if ( projectWindow->projectFileName == "" ) {
        index = tab->currentIndex();
        source = (SourceWindow *)tab->widget(index);
        name = source->fileName;
        if ( name == "" ) {
            int ret = QMessageBox::warning(this, tr("Warning"),
                    tr("This file has not been named.\n"
                        "Do you want save the file?"),
                    QMessageBox::Save 
                    | QMessageBox::Cancel, QMessageBox::Save);
            if ( ret == QMessageBox::Save ) save();
            return;
        }
        sourceFiles << name;
        //qDebug() << "bpts" << *(source->breakpoints);
        index = name.lastIndexOf('.');
        if ( index == -1 ) {
            QMessageBox::warning(this, tr("Warning"),
                    tr("The file name, ") + name +
                    tr(", lacks an extension."),
                    QMessageBox::Ok, QMessageBox::Ok); 
            return;
        } 
        exeName = name;
        exeName.truncate(index);
    } else {
        sourceFiles = projectWindow->fileNames;
        sourceFiles << "ebe_unbuffer.cpp";
        exeName = projectWindow->projectFileName;
        index = exeName.lastIndexOf('.');
        if ( index == -1 ) {
            QMessageBox::warning(this, tr("Warning"),
                    tr("The project file name, ") + exeName +
                    tr(", lacks the .ebe extension."),
                    QMessageBox::Ok, QMessageBox::Ok); 
            return;
        } 
        exeName.truncate(index);
    }

#ifdef Q_WS_WIN
    exeName += ".exe";
#endif

    //
    //  Compile all source files
    //
    //qDebug() << "Files" << sourceFiles;
    //qDebug() << "exe" << exeName;

    QStringList extraCmds;
    QString extraCmd;
    QStringList asmFiles;
    foreach ( name, sourceFiles ) {
        //name = QDir::current().relativeFilePath(name);
        saveIfChanged(name);
        index = name.lastIndexOf('.');
        if ( index == -1 ) continue;
        length = name.length();
        ext = name.right(length-index-1);
        base = name.left(index);
        //qDebug() << name << base << ext;
	qDebug() << "exts" << asmExts;
        if ( cppExts.contains(ext) ) {
            //qDebug() << name << "cpp";
            cmd = ebe["build/cpp"].toString();
            cmd.replace("$base",base);
            cmd.replace("$source",name);
            //qDebug() << cmd;
        } else if ( cExts.contains(ext) ) {
            //qDebug() << name << "c";
            cmd = ebe["build/cc"].toString();
            cmd.replace("$base",base);
            cmd.replace("$source",name);
            //qDebug() << cmd;
        } else if ( asmExts.contains(ext) ) {
            //qDebug() << name << "asm";
            cmd = ebe["build/asm"].toString();
#if defined Q_OS_MAC || defined Q_WS_WIN
            QString debugName = buildDebugAsm(name);
            cmd.replace("$base",base);
            cmd.replace("$source",debugName);
            extraCmd = ebe["build/asmlst"].toString();
            extraCmd.replace("$base",base);
            extraCmd.replace("$source",name);
            extraCmds << extraCmd;
            FileLine fl;
            int n = base.lastIndexOf("/");
            asmFiles.append(base.mid(n+1));
            fl.file = base.mid(n+1);
            fl.line = 0;
            fileLineToAddress[fl] = 0;
#else
            cmd.replace("$base",base);
            cmd.replace("$source",name);
#endif
	    //qDebug() << "copying ebe.inc";
	    QFile::remove("ebe.inc");
	    QFile::copy(":/src/assembly/ebe.inc","ebe.inc");
	    QFile::setPermissions("ebe.inc",
               QFile::ReadOwner | QFile::WriteOwner);
            //qDebug() << cmd;
        } else if ( fortranExts.contains(ext) ) {
            //qDebug() << name << "fortran";
            cmd = ebe["build/fortran"].toString();
            cmd.replace("$base",base);
            cmd.replace("$source",name);
            //qDebug() << cmd;
        }
        object = base + "." + ebe["build/obj"].toString();
        objectFiles << StringPair(object,ext);
        //qDebug() << "cccmd" << cmd;
        QProcess compile(this);
        compile.start(cmd);
        compile.waitForFinished();
        QByteArray data = compile.readAllStandardError();
        if ( data.length() > 0 ) {
            if ( errorWindow.isNull() ) errorWindow = new ErrorWindow;
            errorWindow->setWindowTitle("Errors in " + name );
            errorWindow->textEdit->setPlainText(QString(data));
            errorWindow->show();
            return;
        }
        foreach ( extraCmd, extraCmds ) {
            compile.start(extraCmd);
            compile.waitForFinished();
        }

    }

    //
    //  Examine object files looking for main or _start or start
    //  and building list of globals
    //
    //qDebug() << "building globals";
    QString ldCmd = "";
    textLabels.clear();
    foreach ( StringPair pair, objectFiles ) {
        object = pair.first;
        if ( object == "ebe_unbuffer.o" ) continue;
        ext = pair.second;
        //qDebug() << "object" << object << ext;
        QProcess nm(this);
        nm.start ( "nm -a \"" + object + "\"" );
        //qDebug() << "nm -a \"" + object + "\"";
        nm.waitForFinished();
        nm.setReadChannel(QProcess::StandardOutput);
        QString data = nm.readLine();
        QStringList parts;
	    //qDebug() << data;
        while ( data != "" ) {
            //qDebug() << data;
            parts = data.split(QRegExp("\\s+"));
            //qDebug() << parts;
            if ( parts.length() >= 3 ) {
                if ( parts[1] == "T" && (parts[2] == "main" || parts[2] == "_main") ) {
                    //qDebug() << "found main" << object;
                    if ( cExts.contains(ext) ) {
                        ldCmd = ebe["build/ccld"].toString();
                    } else if ( cppExts.contains(ext) ) {
                        ldCmd = ebe["build/cppld"].toString();
                    } else if ( asmExts.contains(ext) ) {
                        ldCmd = ebe["build/ccld"].toString();
                    } else if ( fortranExts.contains(ext) ) {
                        ldCmd = ebe["build/fortranld"].toString();
                    }
                } else if ( parts[1] == "B" || parts[1] == "D" ||
                            parts[1] == "G" || parts[1] == "C" ) {
#if defined Q_OS_MAC || defined Q_WS_WIN
                    if ( parts[2].at(0) == '_' ) {
                        globals.append(parts[2].mid(1));
                    } else {
                        globals.append(parts[2]);
                    }
#else
                    globals.append(parts[2]);
#endif
                }
                if ( asmExts.contains(ext) && (parts[1] == "T" || parts[1] == "t") &&
                     parts[2].indexOf(".") < 0 ) {
                    textLabels.insert(parts[2]);
                }
            }
            //qDebug() << data;
            data = nm.readLine();
        }
//#ifndef Q_WS_WIN
        if ( asmExts.contains(ext) ) {
            FileLine fl;
            Range range;
            QString labelToSave;
            int m = object.lastIndexOf(".");
            QString name;
            if ( m < 0 ) name = object + "." + ext;
            else name = object.left(m) + "." + ext;
            QFile source(name);
            fl.file = name;
            if ( source.open(QFile::ReadOnly) ) {
                QString text;
                QString label;
                QStringList parts;
                text = source.readLine();
                int line = 1;
                while ( text != "" ) {
                    parts = text.split(QRegExp("\\s+"));
                    if ( parts.length() > 0 ) {
                        label = parts[0];
                        if ( label == "" && parts.length() > 1 ) label = parts[1];
                        int n = label.length();
                        if ( n > 0 ) {
                            if ( label[n-1] == QChar(':') ) label.chop(1);
                            if ( textLabels.contains("_"+label) ) {
                                label = "_" + label;
                            }
                            if ( textLabels.contains(label) ) {
                                if ( labelToSave != "" ) {
                                    range.last = line-1;
                                    labelRange[labelToSave] = range;
                                }
                                range.first = line;
                                labelToSave = label;
                            }
                        }
                    }
                    if ( (parts.length() == 2 && parts[0].toUpper() == "CALL" ) ||
                         (parts.length() == 3 && parts[1].toUpper() == "CALL" ) ||
                         (parts.length() == 4 && parts[2].toUpper() == "CALL" ) ) {
                        fl.line = line;
                        callLines.insert(fl);
                    }
                    text = source.readLine();
                    line++;
                }
                if ( labelToSave != "" ) {
                    range.last = line-1;
                    labelRange[labelToSave] = range;
                }
            }
            source.close();
        }
//#endif
    }
    globals.sort();
    //qDebug() << "labels" << textLabels;

    //foreach ( QString label, labelRange.keys() ) {
        //qDebug() << label << labelRange[label].first << labelRange[label].last;
    //}

    if ( ldCmd == "" ) {
        QMessageBox::warning(this, tr("Warning"),
                tr("None of the source files defines main"),
                QMessageBox::Ok, QMessageBox::Ok); 
        return;
    } 
    //
    //  Link object files to produce executable file
    //
    foreach ( StringPair pair, objectFiles ) {
        ldCmd += " \"" + pair.first + "\"";
    }
    ldCmd += " " + ebe["build/libs"].toString();
    //qDebug() << "ld cmd" << ldCmd;


    QProcess ld(this);
    ldCmd.replace("$base",exeName);
    //qDebug() << "ld cmd" << ldCmd;
    ld.start ( ldCmd );
    ld.waitForFinished();
    ld.setReadChannel(QProcess::StandardError);

    QByteArray data = ld.readAllStandardError();
    if ( data.length() > 0 ) {
        if ( errorWindow.isNull() ) errorWindow = new ErrorWindow;
        errorWindow->setWindowTitle("Link errors");
        errorWindow->textEdit->setPlainText(QString(data));
        errorWindow->show();
        return;
    }


//
//  Discover addresses of globals
//
    QProcess nm(this);
    nm.start(QString("nm -a %1").arg(exeName));
    nm.waitForFinished();
    QString nmData = nm.readLine();
    QStringList nmParts;
    QStringList parts;
    bool ok;
    QRegExp rx1("\\s+");
    QRegExp rx2("[a-zA-Z_][.a-zA-Z0-9_]*");
    QRegExp rx3("_line_");
    FileLine fl;
    while ( nmData != "" ) {
        //qDebug() << nmData;
        nmParts = nmData.split(rx1);
        if ( nmParts.length() >= 3 ) {
            if ( nmParts[1] == "D" || nmParts[1] == "S" ||
                 nmParts[1] == "d" || nmParts[1] == "s" ||
                 nmParts[1] == "B" ) {
                if ( rx2.exactMatch(nmParts[2]) ) {
                    varToAddress[nmParts[2]] = "0x" + nmParts[0];
                }
            } else if ( nmParts[1] == "T" || nmParts[1] == "t" ) {
                parts = nmParts[2].split(QChar('.'));
                if ( parts.length() == 2 ) {
                    parts = parts[1].split(rx3);
                    if ( parts.length() == 2 ) {
                        fl.file = parts[0];
                        fl.line = parts[1].toInt();
                        fileLineToAddress[fl] = nmParts[0].toLong(&ok,16);
                        //qDebug() << fl.file << fl.line << fileLineToAddress[fl];
                    }
                }
            }
        }
        nmData = nm.readLine();
    }
    //qDebug() << "globals" << varToAddress.keys();

    //
    //  Start debugging
    //
    sourceFiles.clear();

    QString s;
    for ( index = 0; index < tab->count(); index++ ) {
        source = (SourceWindow *)tab->widget(index);
        sourceFiles.append ( source->fileName );
#if defined Q_OS_MAC || defined Q_WS_WIN
        QString base;
        QString ext;
        FileLine fl;
        long address;
        QMap<FileLine,unsigned long>::const_iterator it;
        int n = source->fileName.lastIndexOf("/");
        if ( n >= 0 ) {
            base = source->fileName.mid(n+1);
            n = base.lastIndexOf(".");
            if ( n >= 0 ) {
                ext = base.mid(n+1);
                base = base.left(n);
            }
        }
        fl.file = base;
#endif
        bps.clear();
        foreach ( int bp, *(source->breakpoints) ) {
#if defined Q_OS_MAC || defined Q_WS_WIN
            if ( asmExts.contains(ext) ) {
                fl.line = bp;
                it = fileLineToAddress.lowerBound(fl);
                if ( it.value() == 0L ) it = fileLineToAddress.upperBound(fl);
                address = it.value();
                bps.insert(QString("*0x%1").arg(address,1,16));
            } else {
                bps.insert(s.setNum(bp));
            }
#else
            bps.insert(s.setNum(bp));
#endif
        }
        breakpoints.append ( bps );
    }
    //qDebug() << "doRun" << sourceFiles << breakpoints;
    emit doRun(exeName,commandLine->text(),sourceFiles,breakpoints,globals);
}

QString SourceFrame::buildDebugAsm ( QString fileName )
{
    QString debugFileName;
    int n = fileName.lastIndexOf("/");
    if ( n >= 0 ) {
        debugFileName = fileName.left(n) + "/debug_" + fileName.mid(n+1);
    } else {
        debugFileName = QString("debug_") + fileName;
    }
    //qDebug() << "buildDebugAsm" << debugFileName;

    QString base = fileName.mid(n+1);
    n = base.lastIndexOf(".");
    base = base.left(n);

    //qDebug() << "base" << base;

    QFile in(fileName);
    QFile outFile(debugFileName);
    if ( !in.open(QFile::ReadOnly) ) {
        qDebug() << "failed to open" << fileName;
        return "";
    }
    if ( !outFile.open(QFile::WriteOnly) ) {
        qDebug() << "failed to open" << debugFileName;
        return "";
    }
    QTextStream out(&outFile);
    QString text;
    text = in.readLine();
    int line = 1;
    out << "ebedebug:\n";
    while ( text != "" ) {
        out << QString(".%2_line_%3:\n").arg(base).arg(line);
        out << text;
        text = in.readLine();
        line++;
    }
    in.close();
    out.flush();
    outFile.close();
    return debugFileName;
}



void SourceFrame::next()
{
    clearNextLine(breakFile,breakLine);
    emit doNext();
}

void SourceFrame::step()
{
    clearNextLine(breakFile,breakLine);
    emit doStep();
}

void SourceFrame::Continue()
{
    clearNextLine(breakFile,breakLine);
    emit doContinue();
}

void SourceFrame::stop()
{
    clearNextLine(breakFile,breakLine);
    emit doStop();
}

void SourceFrame::clearNextLine ( QString file, int line )
{
    //qDebug() << "cnl" << file << line;
    if ( file == "" || line < 1 ) return;
    for ( int index=0; index < tab->count(); index++ ) {
        source = (SourceWindow *)tab->widget(index);
        if ( source->fileName == QDir::current().absoluteFilePath(file) ) {
            source->clearNextLine(line);
            return;
        }
    }
    breakFile = "";
    breakLine = 0;
}

void SourceFrame::setNextLine ( QString &file, int & line )
{
    //qDebug() << "snl" << file << line;
    if ( file == "" || line < 1 ) return;
    for ( int index=0; index < tab->count(); index++ ) {
        source = (SourceWindow *)tab->widget(index);
        //qDebug() << "source name" << source->fileName << file;
        if ( source->fileName == QDir::current().absoluteFilePath(file) ) {
            tab->setCurrentIndex(index);
            source->setNextLine(line);
            return;
        } 
#if defined Q_OS_MAC || defined Q_WS_WIN
        if ( file.indexOf(".") < 0 ) {
            //FileLine fl(file,line);
            //FileLine fl2(file,line+1);
            //QMap<FileLine,long>::const_iterator it;
            //QMap<FileLine,long>::const_iterator it2;
            //it = fileLineToAddress.lowerBound(fl);
            //it2 = fileLineToAddress.lowerBound(fl2);
            //qDebug() << it.value() << it2.value();
            //while ( it.value() == it2.value() ) {
                //line++;
                //fl.line = line;
                //fl2.line = line+1;
                //it = fileLineToAddress.lowerBound(fl);
                //it2 = fileLineToAddress.lowerBound(fl2);
                //qDebug() << it.value() << it2.value();
            //}
            int n = source->fileName.lastIndexOf("/");
            if ( n < 0 ) continue;
            QString base = source->fileName.mid(n+1);
            n = base.lastIndexOf(".");
            if ( n < 0 ) continue;
            QString ext = base.mid(n+1);
            if ( !asmExts.contains(ext) ) continue;
            base = base.left(n);
            if ( base == file ) {
                file = source->fileName;
                tab->setCurrentIndex(index);
                source->setNextLine(line);
                return;
            }
        }
#endif
    }
}

void SourceFrame::nextInstruction ( QString file, int line )
{
    //qDebug() << "nextI" << file << line;
    if ( breakFile != "" ) {
        clearNextLine(breakFile,breakLine);
    }
    if ( file != "" ) breakFile = file;
    breakLine = line;
    setNextLine(breakFile,breakLine);
}

void SourceFrame::setFontHeightAndWidth ( int height, int width )
{
    int current = tab->currentIndex();
    int count = tab->count();
    for ( int i = 0; i < count; i++ ) {
        source = (SourceWindow *)tab->widget(i);
        source->setFontHeightAndWidth(height,width);
    }
    tab->setCurrentIndex(current);
    fontHeight = height;
    fontWidth = width;
}

void SourceFrame::updateCursorPosition()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source == 0 ) return;
    source->setLineNumbers(source->textEdit->document()->lineCount());
    QTextCursor cursor = source->textEdit->textCursor();
    QTextBlock block = cursor.block();
    int row = block.blockNumber()+1;
    int column = cursor.positionInBlock()+1;
    cursorPosition->setText(QString("line %1, column %2").arg(row).arg(column));
}

void SourceFrame::changedTab(int index)
{
    //qDebug() << "switching to " << index;
    source = (SourceWindow *)tab->widget(index);
    updateCursorPosition();
}

void SourceFrame::openInNewTab(QString /*name*/)
{
}

void SourceFrame::open(QString name,int index)
{
    if ( index == -1 ) index = tab->count();
    while (index >= tab->count()) {
        source = new SourceWindow;
        index = tab->addTab(source,"");
    }
    source = (SourceWindow *)tab->widget(index);
    source->open(name);
    if ( source && source->opened ) {
        name = QDir::current().relativeFilePath(name);
        //qDebug() << "Setting name " << index << name;
        tab->setTabText(index,name);
    }
}

void SourceFrame::closeTabs()
{
}

void SourceFrame::setCommandLineVisible(bool visible)
{
    ebe["command/visible"] = visible;
    commandLine->setVisible(visible);
}

void SourceFrame::open(bool /* checked */)
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    //qDebug() << "open" << index << source << source->textDoc->characterCount();
    if ( !source || source->textDoc->characterCount() > 1 ) {
        source = new SourceWindow;
        index = tab->addTab(source,"");
        tab->setCurrentIndex(index);
    }
    source->open();
    if ( source && source->opened ) {
        QString name = QDir::current().absoluteFilePath(source->fileName);
        name = QDir::current().relativeFilePath(name);
        //qDebug() << "Setting name " << index << name;
        tab->setTabText(index,name);
    }
}

void SourceFrame::close()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        if ( source->changed ) {
            int ret = QMessageBox::warning(this, tr("Warning"),
                    tr("This file has changed.\n"
                        "Do you want save the file?"),
                    QMessageBox::Save | QMessageBox::Discard
                    | QMessageBox::Cancel, QMessageBox::Save);
            switch ( ret ) {
                case QMessageBox::Save:
                    save();
                    source->close();
                    delete source;
                    break;
                case QMessageBox::Discard:
                    //tab->removeTab(index);
                    source->close();
                    delete source;
            }
        } else {
            //tab->removeTab(index);
            source->close();
            delete source;
        }
    }
}

void SourceFrame::close(QString name)
{
    //qDebug() << "sf close" << name;
    for ( int index = 0; index < tab->count(); index++ ) {
        source = (SourceWindow *)tab->widget(index);
        //qDebug() << index << source->fileName;
        if ( source && source->fileName == name ) {
            //qDebug() << "closing" << index << source->fileName;
            if ( source->fileChanged() ) {
                source->save();
            }
            source->close();
            delete source;
            return;
        }
    }
}

void SourceFrame::save()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->save();
        if ( source->saved ) {
            QString name = source->fileName;
            name = QDir::current().relativeFilePath(name);
            //qDebug() << "Setting name " << index << name;
            tab->setTabText(index,name);
        }
    }
}

void SourceFrame::saveAs()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->saveAs();
        if ( source->saved ) {
            QString name = source->fileName;
            name = QDir::current().relativeFilePath(name);
            //qDebug() << "Setting name " << index << name;
            tab->setTabText(index,name);
        }
    }
}

bool SourceFrame::filesSaved()
{
    QStringList filesToSave;
    QList<SourceWindow*> windows;
    SourceWindow *window;

    for ( int i = 0; i < tab->count(); i++ ) {
        window = (SourceWindow *)tab->widget(i);
        window->saveCursor();
        if ( window->changed &&
                window->textEdit->document()->characterCount() > 1 ) {
            filesToSave.append(window->fileName);
            windows.append(window);
        }
    }

    if ( filesToSave.length() > 0 ) {
        QString msg;
        msg = "The following files have not been saved:\n";
        foreach ( QString name, filesToSave ) {
            msg += "        " + name + "\n";
        }
        msg += "Do you wish to save them all?";
        int ret = QMessageBox::warning(this, tr("Warning"), msg,
                QMessageBox::Save | QMessageBox::Discard |
                QMessageBox::Cancel,
                QMessageBox::Save);
        if ( ret == QMessageBox::Discard ) return true;
        if ( ret == QMessageBox::Cancel ) return false;
        foreach ( window, windows ) {
            window->save();
            if ( window->changed ) return false;
        }
    }
    return true;
}

void SourceFrame::cut()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->textEdit->cut();
    }
}

void SourceFrame::copy()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->textEdit->copy();
    }
}

void SourceFrame::paste()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->textEdit->paste();
    }
}

void SourceFrame::undo()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->textEdit->undo();
    }
}

void SourceFrame::redo()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->textEdit->redo();
    }
}

void SourceFrame::selectAll()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->textEdit->selectAll();
    }
}

void SourceFrame::selectNone()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        QTextCursor tc = source->textEdit->textCursor();
        tc.clearSelection();
        source->textEdit->setTextCursor(tc);
        gotoFirstLine();
    }
}

void SourceFrame::comment()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->comment();
    }
}

void SourceFrame::unComment()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->unComment();
    }
}

void SourceFrame::indent()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->indent();
    }
}

void SourceFrame::unIndent()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->unIndent();
    }
}

void SourceFrame::pageForward()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->pageForward();
    }
}

void SourceFrame::pageBackward()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->pageBackward();
    }
}

void SourceFrame::center()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->center();
    }
}

void SourceFrame::gotoFirstLine()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->gotoFirstLine();
    }
}

void SourceFrame::gotoLastLine()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->gotoLastLine();
    }
}

void SourceFrame::gotoTop()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->gotoTop();
    }
}

void SourceFrame::gotoBottom()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->gotoBottom();
    }
}

void SourceFrame::gotoLine()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->gotoLine();
    }
}

void SourceFrame::prettify()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->prettify();
    }
}

void SourceFrame::newFile()
{
    source = new SourceWindow;
    int index = tab->addTab(source,"unnamed");
    tab->setCurrentIndex(index);
}

void SourceFrame::templateC()
{
    int index;
    if ( !source || source->textEdit->document()->characterCount() > 1 ) {
        source = new SourceWindow;
        index = tab->addTab(source,"hello.c");
        tab->setCurrentIndex(index);
    }
    index = tab->currentIndex();
    tab->setTabText(index,"hello.c");
    QFile::remove("hello.c");
    QFile::copy(":/src/c/hello.c","hello.c");
    QFile::setPermissions("hello.c", QFile::ReadOwner | QFile::WriteOwner);
    source->open("hello.c");
    source->setFontHeightAndWidth(fontHeight,fontWidth);
}

void SourceFrame::templateCpp()
{
    if ( !source || source->textEdit->document()->characterCount() > 1 ) {
        source = new SourceWindow;
        int index = tab->addTab(source,"hello.cpp");
        tab->setCurrentIndex(index);
    }
    int index = tab->currentIndex();
    tab->setTabText(index,"hello.cpp");
    QFile::remove("hello.cpp");
    QFile::copy(":/src/cpp/hello.cpp","hello.cpp");
    QFile::setPermissions("hello.cpp", QFile::ReadOwner | QFile::WriteOwner);
    source->open("hello.cpp");
    source->setFontHeightAndWidth(fontHeight,fontWidth);
}

void SourceFrame::templateAssembly()
{
    if ( !source || source->textEdit->document()->characterCount() > 1 ) {
        source = new SourceWindow;
        int index = tab->addTab(source,"hello.asm");
        tab->setCurrentIndex(index);
    }
    int index = tab->currentIndex();
    tab->setTabText(index,"hello.asm");
    QFile::remove("hello.asm");
    QFile::copy(
        QString(":/src/assembly/%1/hello.asm").arg(ebe.os),
        "hello.asm");
    QFile::setPermissions("hello.asm", QFile::ReadOwner | QFile::WriteOwner);
    source->open("hello.asm");
    source->setFontHeightAndWidth(fontHeight,fontWidth);
}

void SourceFrame::templateFortran()
{
    if ( !source || source->textEdit->document()->characterCount() > 1 ) {
        source = new SourceWindow;
        int index = tab->addTab(source,"hello.f");
        tab->setCurrentIndex(index);
    }
    int index = tab->currentIndex();
    tab->setTabText(index,"hello.f");
    QFile::remove("hello.f");
    QFile::copy(":/src/fortran/hello.f","hello.f");
    QFile::setPermissions("hello.f", QFile::ReadOwner | QFile::WriteOwner);
    source->open("hello.f");
    source->setFontHeightAndWidth(fontHeight,fontWidth);
}

void SourceFrame::find()
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->find();
    }
}

void SourceFrame::insertFile(QString f)
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( source ) {
        source->insertFile(f);
    }
}

void SourceFrame::doTemplate(QAction *a)
{
    int index = tab->currentIndex();
    source = (SourceWindow *)tab->widget(index);
    if ( a->text() == "library" ) {
        if ( libraryWindow.isNull() ) {
            libraryWindow = new LibraryWindow;
        }
        libraryWindow->show();
        libraryWindow->setWindowState(Qt::WindowActive);
        return;
    }
    if ( source ) {
        source->doTemplate(a);
    }
}
