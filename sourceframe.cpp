#include "sourceframe.h"
#include "sourcewindow.h"
#include "projectwindow.h"
#include "commandline.h"
#include "stylesheet.h"
#include "settings.h"

extern ProjectWindow *projectWindow;

SourceFrame::SourceFrame(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    initStyleSheet("sourceframe","QPushButton { font-family: " +
                   ebe["variable_font"].toString() + "}" +
                   "QLabel { font-family:" +
                   ebe["variable_font"].toString() + "}" );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(10,10,10,10);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(6);
    buttonLayout->setContentsMargins(2,2,2,2);

    quitButton     = new QPushButton ( "Quit" );
    quitButton->setStyleSheet ( "color: "+ebe["quit_color"].toString() );
    quitButton->setToolTip ( tr("Click this button to exit from ebe") );
    runButton      = new QPushButton ( "Run" );
    runButton->setToolTip ( tr("Compile and run your program") );
    runButton->setStyleSheet ( "color: "+ebe["run_color"].toString() );
    nextButton     = new QPushButton ( "Next" );
    nextButton->setToolTip (
        tr("Execute the current statement in the same function") );
    nextButton->setStyleSheet ( "color: "+ebe["next_color"].toString() );
    stepButton     = new QPushButton ( "Step" );
    stepButton->setStyleSheet ( "color: "+ebe["step_color"].toString() );
    stepButton->setToolTip (
        tr("Execute the current statement possibly stepping\n"
           "into a different function") );
    continueButton = new QPushButton ( "Continue" );
    continueButton->setStyleSheet ( "color: "+ebe["continue_color"].toString() );
    continueButton->setToolTip (
        tr("Execute statements starting at the current statement\n"
           "continuing until the program ends or a breakpoint\n"
           "is reached.") );
    stopButton     = new QPushButton ( "Stop" );
    stopButton->setStyleSheet ( "color: "+ebe["stop_color"].toString() );
    stopButton->setToolTip (
        tr("End this debugging session and continue editing") );
    buttonLayout->addWidget ( quitButton );
    buttonLayout->addWidget ( runButton );
    buttonLayout->addWidget ( nextButton );
    buttonLayout->addWidget ( stepButton );
    buttonLayout->addWidget ( continueButton );
    buttonLayout->addWidget ( stopButton );
    buttonLayout->addStretch();

    connect ( quitButton, SIGNAL(clicked()), parent, SLOT(quit()) );
    connect ( runButton, SIGNAL(clicked()), this, SLOT(run()) );

    commandLine = new CommandLine();
    commandLine->setToolTip (
        tr("This input field is for executing your program with\n"
           "command line parameters to be accepted in the argv\n"
           "array") );

    tab = new QTabWidget;

    layout->addLayout(buttonLayout);
    layout->addWidget(commandLine);
    layout->addWidget(tab);

    connect ( tab, SIGNAL(currentChanged(int)), this, SLOT(changedTab(int)) );

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

void SourceFrame::run()
{
    int index;
    int length;
    QStringList sourceFiles;
    QString exeName;
    QString ext;
    QString name;
    QString base;
    QString cmd;

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
        index = name.lastIndexOf('.');
        if ( index == -1 ) {
            int ret = QMessageBox::warning(this, tr("Warning"),
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
            int ret = QMessageBox::warning(this, tr("Warning"),
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
    qDebug() << "Files" << sourceFiles;
    qDebug() << "exe" << exeName;

    foreach ( name, sourceFiles ) {
        name = QDir::current().relativeFilePath(name);
        index = name.lastIndexOf('.');
        if ( index == -1 ) continue;
        length = name.length();
        ext = name.right(length-index-1);
        base = name.left(index);
        qDebug() << name << base << ext;
        if ( cppExts.contains(ext) ) {
            qDebug() << name << "cpp";
            cmd = ebe["build/cpp"].toString();
            cmd.replace("$base",base);
            cmd.replace("$source",name);
            qDebug() << cmd;
        } else if ( cExts.contains(ext) ) {
            qDebug() << name << "c";
            cmd = ebe["build/cc"].toString();
            cmd.replace("$base",base);
            cmd.replace("$source",name);
            qDebug() << cmd;
        } else if ( asmExts.contains(ext) ) {
            qDebug() << name << "asm";
            cmd = ebe["build/asm"].toString();
            cmd.replace("$base",base);
            cmd.replace("$source",name);
            qDebug() << cmd;
        } else if ( fortranExts.contains(ext) ) {
            qDebug() << name << "fortran";
            cmd = ebe["build/fortran"].toString();
            cmd.replace("$base",base);
            cmd.replace("$source",name);
            qDebug() << cmd;
        }
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
            QMessageBox::critical(this, tr("Errors in ") + name,
                                data,
                                QMessageBox::Ok, QMessageBox::Ok); 
        }
    }
//
//  Link object files to produce executable file
//

//
//  Start debugging
//
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
}

void SourceFrame::changedTab(int index)
{
    qDebug() << "switching to " << index;
    source = (SourceWindow *)tab->widget(index);
}

void SourceFrame::openInNewTab(QString name)
{
}

void SourceFrame::open(QString name,int index)
{
    while (index >= tab->count()) {
        source = new SourceWindow;
        int index = tab->addTab(source,"");
    }
    source = (SourceWindow *)tab->widget(index);
    source->open(name);
    if ( source && source->opened ) {
        name = QDir::current().relativeFilePath(name);
        qDebug() << "Setting name " << index << name;
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

void SourceFrame::open()
{
    int index = tab->currentIndex();
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
        qDebug() << "Setting name " << index << name;
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
                delete source;
            }
        } else {
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
            qDebug() << "Setting name " << index << name;
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
            qDebug() << "Setting name " << index << name;
            tab->setTabText(index,name);
        }
    }
}
