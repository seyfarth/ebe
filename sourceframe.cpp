#include "sourceframe.h"
#include "projectwindow.h"
#include "commandline.h"
#include "stylesheet.h"
#include "errorwindow.h"
#include "gdb.h"
#include "settings.h"

QStringList fortranExts;
QStringList cExts;
QStringList cppExts;
QStringList asmExts;

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

    connect ( this, SIGNAL(doRun(QString,QString,QStringList,QList<IntSet>,QStringList)),
            gdb, SLOT(doRun(QString,QString,QStringList,QList<IntSet>,QStringList)) );
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
    QStringList sourceFiles;
    QList<IntSet> breakpoints;
    QList<StringPair> objectFiles;
    QString object;
    QString exeName;
    QString ext;
    QString name;
    QString base;
    QString cmd;

    breakLine = 0;
    breakFile = "";

    //
    //  Determine all source files
    //
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

    //
    //  Compile all source files
    //
    //qDebug() << "Files" << sourceFiles;
    //qDebug() << "exe" << exeName;

    foreach ( name, sourceFiles ) {
        name = QDir::current().relativeFilePath(name);
        saveIfChanged(name);
        index = name.lastIndexOf('.');
        if ( index == -1 ) continue;
        length = name.length();
        ext = name.right(length-index-1);
        base = name.left(index);
        //qDebug() << name << base << ext;
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
            cmd.replace("$base",base);
            cmd.replace("$source",name);
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
        QProcess compile(this);
        compile.start(cmd);
        compile.waitForFinished();
        compile.setReadChannel(QProcess::StandardError);
        QString data;
        unsigned char s[1025];
        int n;
        while ( (n = compile.readLine((char *)s,1024)) > 0 ) {
            for ( int i = 0; i < n; i++ ) {
                if ( s[i] < 128 ) data += s[i];
            }
        }
        if ( data.length() > 0 ) {
            if ( errorWindow.isNull() ) errorWindow = new ErrorWindow;
            errorWindow->setWindowTitle("Errors in " + name );
            errorWindow->textEdit->setPlainText(data);
            errorWindow->show();
            return;
        }
    }
    //
    //  Examine object files looking for main or _start or start
    //
    QString ldCmd = "";
    foreach ( StringPair pair, objectFiles ) {
        object = pair.first;
        ext = pair.second;
        //qDebug() << "object" << object << ext;
        QProcess nm(this);
        nm.start ( "nm -g " + object );
        nm.waitForFinished();
        nm.setReadChannel(QProcess::StandardOutput);
        QString data = nm.readLine();
        QStringList parts;
        while ( data != "" ) {
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
                        ldCmd = ebe["build/asmld"].toString();
                    } else if ( fortranExts.contains(ext) ) {
                        ldCmd = ebe["build/fortranld"].toString();
                    }
                    break;
                }
            }
            //qDebug() << data;
            data = nm.readLine();
        }
    }

    if ( ldCmd == "" ) {
        QMessageBox::warning(this, tr("Warning"),
                tr("None of the source files defines main"),
                QMessageBox::Ok, QMessageBox::Ok); 
        return;
    } 
    //qDebug() << "ld cmd" << ldCmd;
    //
    //  Link object files to produce executable file
    //
    foreach ( StringPair pair, objectFiles ) {
        ldCmd += " " + pair.first;
    }


    QProcess ld(this);
    ldCmd.replace("$base",exeName);
    //qDebug() << "ld cmd" << ldCmd;
    ld.start ( ldCmd );
    ld.waitForFinished();
    ld.setReadChannel(QProcess::StandardError);

    QString data;
    unsigned char s[1025];
    int n;
    while ( (n = ld.readLine((char *)s,1024)) > 0 ) {
        for ( int i = 0; i < n; i++ ) {
            if ( s[i] < 128 ) data += s[i];
        }
    }
    if ( data.length() > 0 ) {
        if ( errorWindow.isNull() ) errorWindow = new ErrorWindow;
        errorWindow->setWindowTitle("Link errors");
        errorWindow->textEdit->setPlainText(data);
        errorWindow->show();
        return;
    }

    //
    //  Inspect exe file with nm to determine globals in .data and .bss
    //  "B" "D" and "G" for small objects
    //
    QString nmCmd = QString("nm -C --defined %1").arg(exeName);
    QProcess nm(this);
    //qDebug() << "nm cmd" << nmCmd;
    nm.start ( nmCmd );
    nm.waitForFinished();
    ld.setReadChannel(QProcess::StandardOutput);

    QStringList globals;
    QStringList parts;
    QRegExp rx("^[A-Za-z][A-Za-z0-9_]*$");
    while ( (n = nm.readLine((char *)s,1024)) > 0 ) {
        data = "";
        for ( int i = 0; i < n; i++ ) {
            if ( s[i] < 128 ) data += s[i];
        }
        parts = data.split(QRegExp("\\s+"));
        //qDebug() << "parts" << parts;
        if ( parts.length() >= 3 ) {
            if ( parts[1] == "B" || parts[1] == "b" ||
                    parts[1] == "D" || parts[1] == "d" ||
                    parts[1] == "G" || parts[1] == "g" ) {
                if ( rx.indexIn(parts[2]) >= 0 ) {
                    globals.append(parts[2]);
                }
            }
        }
    }
    globals.sort();
    //qDebug() << "globals" << globals;
    //
    //  Start debugging
    //
    sourceFiles.clear();
    for ( index = 0; index < tab->count(); index++ ) {
        source = (SourceWindow *)tab->widget(index);
        sourceFiles.append ( source->fileName );
        breakpoints.append ( *(source->breakpoints) );
    }
    //qDebug() << "doRun" << sourceFiles << breakpoints;
    emit doRun(exeName,commandLine->text(),sourceFiles,breakpoints,globals);
}

void SourceFrame::next()
{
    emit doNext();
}

void SourceFrame::step()
{
    emit doStep();
}

void SourceFrame::Continue()
{
    emit doContinue();
}

void SourceFrame::stop()
{
    clearNextLine(breakFile,breakLine);
    emit doStop();
}

void SourceFrame::clearNextLine ( QString file, int line )
{
    for ( int index=0; index < tab->count(); index++ ) {
        source = (SourceWindow *)tab->widget(index);
        if ( source->fileName == file ) {
            source->clearNextLine(line);
            return;
        }
    }
}

void SourceFrame::setNextLine ( QString file, int line )
{
    for ( int index=0; index < tab->count(); index++ ) {
        source = (SourceWindow *)tab->widget(index);
        if ( source->fileName == file ) {
            tab->setCurrentIndex(index);
            source->setNextLine(line);
            return;
        }
    }
}

void SourceFrame::nextInstruction ( QString file, int line )
{
    //qDebug() << "break" << file << line;
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
    //qDebug() << "open";
    source = (SourceWindow *)tab->widget(index);
    if ( !source || source->textDoc->characterCount() > 1 ) {
        source = new SourceWindow;
        index = tab->addTab(source,"");
        tab->setCurrentIndex(index);
    }
    source->open();
    if ( source && source->opened ) {
        QString name = source->fileName;
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
                    break;
                case QMessageBox::Discard:
                    tab->removeTab(index);
                    delete source;
            }
        } else {
            tab->removeTab(index);
            delete source;
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
        source->textEdit->moveCursor(QTextCursor::Start,QTextCursor::MoveAnchor);
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
    if ( !source || source->textEdit->document()->characterCount() > 1 ) {
        source = new SourceWindow;
        int index = tab->addTab(source,"hello.c");
        tab->setCurrentIndex(index);
    }
    int index = tab->currentIndex();
    source->textEdit->setPlainText(
            "#include <stdio.h>\n"
            "\n"
            "int main ( int argc, char **argv )\n"
            "    printf ( \"Hello World!\\n\" );\n"
            "    return 0;\n"
            "}"
            );
    source->fileName = "hello.c";
    tab->setTabText(index,"hello.c");
    source->save();
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
    source->textEdit->setPlainText(
            "#include <iostream>\n"
            "\n"
            "using namespace std;\n"
            "\n"
            "int main ( int argc, char **argv )\n"
            "{\n"
            "    cout << \"Hello World!\\n\" << endl;\n"
            "    return 0;\n"
            "}"
            );
    source->fileName = "hello.cpp";
    tab->setTabText(index,"hello.cpp");
    source->save();
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
    source->textEdit->setPlainText(
            "msg:   db      'Hello World!',0x0a     ; String to print\n"
            "len:   equ     $-msg                   ; String length\n"
            "\n"
            "       segment .text\n"
            "       global  _main                   ; Tell linker about main\n"
            "       extern  _write, _exit\n"
            "_main:\n"
            "       push    rbp\n"
            "       mov     rbp. rsp\n"
            "       mov     edx, len                ; Parameter 3 for write\n"
            "       lea     rsi, [msg]              ; Parameter 2 for write\n"
            "       mov     edi, 1                  ; Parameter 1 (fd)\n"
            "       call    _write\n"
            "\n"
            "       xor     edi, edi                ; 0 return = success\n"
            "       call    _exit"
            );
    source->fileName = "hello.asm";
    tab->setTabText(index,"hello.asm");
    source->save();
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
    source->textEdit->setPlainText(
            "       program hello\n"
            "       print *, 'Hello World!'\n"
            "       stop\n"
            "       end"
            );
    source->fileName = "hello.f";
    tab->setTabText(index,"hello.f");
    source->save();
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

