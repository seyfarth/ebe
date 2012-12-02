#include "sourcewindow.h"
#include "sourceedit.h"
#include "commandline.h"
#include "stylesheet.h"
#include "settings.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextCursor>
#include <QApplication>
#include <QKeyEvent>
#include <QPalette>
#include <QScrollBar>

// For open() and other file menu slots
#include <QFileDialog>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QIODevice>
#include <QTextStream>
#include <QByteArray>

#include <QMessageBox>
#include <QtDebug>
#include <cstdio>

SourceWindow::SourceWindow(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    lineNumberEdit = new LineNumberEdit(this);
    createTextEdit();
    initStyleSheet("editor","QPushButton { font-family: " +
                   ebe["variable_font"].toString() + "}" +
                   "QLabel { font-family:" +
                   ebe["variable_font"].toString() + "}" );

    QVBoxLayout *sourceLayout = new QVBoxLayout;
    sourceLayout->setSpacing(2);
    sourceLayout->setContentsMargins(10,10,10,10);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(6);
    buttonLayout->setContentsMargins(2,2,2,2);

    quitButton     = new QPushButton ( "Quit" );
    quitButton->setStyleSheet ( "color: "+ebe["quit_color"].toString() );
    quitButton->setToolTip ( tr("Click this button to exit from ebe") );
    runButton      = new QPushButton ( "Run" );
    runButton->setStyleSheet ( "color: "+ebe["run_color"].toString() );
    nextButton     = new QPushButton ( "Next" );
    nextButton->setStyleSheet ( "color: "+ebe["next_color"].toString() );
    stepButton     = new QPushButton ( "Step" );
    stepButton->setStyleSheet ( "color: "+ebe["step_color"].toString() );
    continueButton = new QPushButton ( "Continue" );
    continueButton->setStyleSheet ( "color: "+ebe["continue_color"].toString() );
    stopButton     = new QPushButton ( "Stop" );
    stopButton->setStyleSheet ( "color: "+ebe["stop_color"].toString() );
    buttonLayout->addWidget ( quitButton );
    buttonLayout->addWidget ( runButton );
    buttonLayout->addWidget ( nextButton );
    buttonLayout->addWidget ( stepButton );
    buttonLayout->addWidget ( continueButton );
    buttonLayout->addWidget ( stopButton );
    buttonLayout->addStretch();

    connect ( quitButton, SIGNAL(clicked()), parent, SLOT(quit()) );

    commandLine = new CommandLine();

    QHBoxLayout *editorLayout = new QHBoxLayout;
    editorLayout->setSpacing(2);
    editorLayout->setContentsMargins(3,3,3,3);

    editorLayout->addWidget(lineNumberEdit,0);
    editorLayout->addWidget(textEdit,1);

    sourceLayout->addLayout(buttonLayout);
    sourceLayout->addWidget(commandLine);
    sourceLayout->addLayout(editorLayout);
    setLayout(sourceLayout);

    heightInPixels = 0;
    textHeight = 0;
    numLines = 0;
    changed = false;
    topNumber = 0;
    bottomNumber = 0;

    scrollBar = textEdit->verticalScrollBar();
    textDoc = textEdit->document();

    // Auto update line number edit control when text changes in main
    // text edit widget
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
    connect ( textEdit, SIGNAL(newHeight(int)), this, SLOT(newHeight(int)));
    //connect ( scrollBar, SIGNAL(sliderMoved(int)),
              //this, SLOT(scrollBarChanged(int)));
}

void SourceWindow::setFontHeightAndWidth ( int height, int width )
{
    fontHeight = height;
    fontWidth  = width;
    lineNumberEdit->setFixedWidth(width*4+12);
    if ( heightInPixels > 0 ) {
        textHeight = heightInPixels/fontHeight;
    } else {
        textHeight = 0;
    }
}

void SourceWindow::scrollBarChanged ( int value )
{
   setLineNumbers(textDoc->lineCount());
}

void SourceWindow::newHeight ( int height )
{
    heightInPixels = height;
    textHeight = heightInPixels / fontHeight;
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::setCommandLineVisible(bool visible)
{
    ebe["command/visible"] = visible;
    commandLine->setVisible(visible);
}

void SourceWindow::createTextEdit()
{
    textEdit = new SourceEdit(this);
    // textEdit->appendPlainText("Text");
}

void SourceWindow::open()
{
    // How shall we set status bar text here?

    // TODO: Add Fortran file extensions and other assembler extensions
    // Any files?
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        ".", tr("Assembly files (*.asm *akefile);;") +
             tr("C/C++ files (*.c *.cpp *.h *.hpp *akefile);;)") +
             tr("Fortran files (*.f *.F *.f* *.F* *akefile);;All files (*.* *)"));

    if (fileName == "")
    {
        qDebug() << "File name is empty";
        return;
    }

    QFile file(fileName);
    QFileInfo fileInfo(file);

    if (not fileInfo.exists())
    {
        qDebug() << "File " << fileName << " does not exist!";
        QMessageBox::critical(this, tr("Error"), tr("File ") + fileName
            + tr(" does not exist."));
        return;
    }

    if (not fileInfo.isReadable())
    {
        QMessageBox::critical(this, tr("Error"), tr("File ") + fileName
            + tr(" is not readable."));
        return;
    }

    if (not fileInfo.isWritable())
    {
        qDebug() << "File opened in read-only mode!";
    }

    if (not file.open(QIODevice::ReadWrite))
    {
        qDebug() << "Could not open file in read/write mode!";
        return;
    }

    //QTextStream input(&file);

    openedFileName = fileName;

    //textEdit->clear();
    int lineCount = 0;
    //while (not file.atEnd())
    //{
    QByteArray text = file.readAll();
    int length = text.count();
    if ( text[length-1] == '\n' ) text.chop(1);
    textEdit->setPlainText(text);
        //lineCount++;
    //}

    file.close();

    lineCount=textEdit->document()->lineCount();
    //setLineNumbers(lineCount);
}

void SourceWindow::save()
{
    if (openedFileName.isEmpty() or not changed)
    {
        qDebug() << "save(): No opened file name or text not changed";
    }

    QString homeDir = QDir::toNativeSeparators(QDir::homePath());

    if (homeDir == "")
    {
        qDebug() << "save(): Home directory could not be found.";
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
        homeDir, tr("Text Files (*.txt);;Assembly Files (*.asm);;"
        "C++ Files (*.cpp *.h *.hpp);;All files (*.*)"));

    if (fileName != "") {

        // Same as currently opened file name?
        if (openedFileName.compare(fileName) == 0)
        {
            qDebug() << "Saving to same file name that was opened...";
        }
        else
        {
            qDebug() << "save()";
            qDebug() << "Not the same file name that was opened...";
        }

        QFile file(fileName);        
        QFileInfo fileInfo(file);

        // If file name selected does not exist, opening with mode
        // QIODevice::Write or QIODevice::ReadWrite will attempt
        // to create the file
        if (not file.open(QIODevice::ReadWrite)) {
            
            qDebug() << "save(): cannot open file read/write!";

            QMessageBox::critical(this, tr("Error"), tr("Failed to open file ") +
                fileName + tr(" for saving."));

            return;
        }
        else {

            if (not fileInfo.isWritable())
            {
                qDebug() << "save(): File is not writable!";

                QMessageBox::critical(this, tr("Error"), tr("File ") + fileName
                    + tr(" is not writable. Save file failed."));

                return;
            }

            QTextStream stream(&file);
            // Could use QTextDocument but makes sense to save as plain text
            stream << textEdit->toPlainText();
            stream.flush();
            file.close();

            // File changed variable, reset to false
            changed = false;
        }
    }

}

void SourceWindow::createButtons()
{
}

bool SourceWindow::fileChanged()
{
    return changed;
}

void SourceWindow::saveBeforeQuit()
{
    save();
}

void SourceWindow::createCommandLineEdit()
{
}

// Connected to signal textChanged
// Then get number of lines from QTextDocument object and update
// line number control
void SourceWindow::textChanged()
{
    // textChanged signal has been emitted
    changed = true;
    textDoc = textEdit->document();
    setLineNumbers(textDoc->lineCount());
}

// Sets the line numbers in the line number edit control
void SourceWindow::setLineNumbers(int nLines)
{
    int top, bottom, limit;
    char s[10];

    if ( nLines <= textHeight ) {
        top = 1;
        bottom = nLines;
        limit = bottom;
    } else {
        top = scrollBar->value() + 1;
        bottom = top + textHeight - 1;
        limit = bottom + 2;
        if ( limit > nLines ) limit = nLines;
    }

    if ( top == topNumber && bottom == bottomNumber ) return;

    // Clear before updating line numbers again
    lineNumberEdit->clear();

    for (int i = top; i <= limit; i++) {
        sprintf(s,"%4d",i);
        lineNumberEdit->appendPlainText(s);
    }

    lineNumberEdit->scrollBar->setValue(0);

    topNumber = top;
    bottomNumber = bottom;
}

LineNumberEdit::LineNumberEdit(QWidget *parent)
: QPlainTextEdit(parent)
{
    setFixedWidth(60);
    setReadOnly(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollBar = verticalScrollBar();
}

void LineNumberEdit::wheelEvent ( QWheelEvent *e )
{
}
