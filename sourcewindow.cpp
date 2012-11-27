#include "sourcewindow.h"
#include "sourceedit.h"
#include "commandline.h"
#include "settings.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextCursor>
#include <QApplication>
#include <QKeyEvent>
#include <QPalette>

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

SourceWindow::SourceWindow(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Box | QFrame::Raised );
    setLineWidth(3);
    setMidLineWidth(1);

    createLineNumberEdit();
    createTextEdit();
    setStyleSheet("QPushButton { font-family: " +
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

    // Auto update line number edit control when text changes in main
    // text edit widget
    connect(textEdit, SIGNAL(textChanged()),
        this, SLOT(prepareLineNumberEdit()));
}

void SourceWindow::createLineNumberEdit()
{
    lineNumberEdit = new QPlainTextEdit(this);
    lineNumberEdit->setFixedWidth(60);
    // lineNumberEdit->appendPlainText(QString("1234"));
    lineNumberEdit->setReadOnly(true);
}

void SourceWindow::setLineNumberWidth ( int width )
{
    lineNumberEdit->setFixedWidth(width);
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

    QString homeDir = QDir::toNativeSeparators(QDir::homePath());

    if (homeDir == "")
    {
        qDebug() << "Home directory could not be found.";
    }

    // TODO: Add Fortran file extensions and other assembler extensions
    // Any files?
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
        homeDir, tr("Assembly Files (*.asm);;C/C++ Files (*.c *.cpp *.h *.hpp)"));

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

    // int lineCount = 1;
    QTextStream input(&file);

    openedFileName = fileName;

    while (not input.atEnd())
    {
        textEdit->appendPlainText(input.readLine());
        // lineCount++;
    }

    file.close();

    // setLineNumbers(lineCount);
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
void SourceWindow::prepareLineNumberEdit()
{
    // textChanged signal has been emitted
    changed = true;

    // Fetch the QTextDocument
    textDoc = textEdit->document();

    // Hopefully this will take care of any newline type
    // \r\n on Windows, \r but not with an \n following on Mac,
    // and it seems \n for everything else, all the Unixes anyway
    // int lineNumbers = text.count(QRegExp("\\r\\n|\\r(?!\\n)|\\n"));

    int numLines = textDoc->lineCount();
    
    // Clear then update the line number edit control
    setLineNumbers(numLines);
}

// Sets the line numbers in the line number edit control
void SourceWindow::setLineNumbers(int nLines)
{
    // Clear before updating line numbers again
    lineNumberEdit->clear();

    for (int i = 1; i <= nLines; i++)
        lineNumberEdit->appendPlainText(QString::number(i, 10));
}

