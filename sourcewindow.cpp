#include "sourcewindow.h"
#include "stylesheet.h"
#include "settings.h"
#include <QtGui>



SourceEdit::SourceEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    setWordWrapMode(QTextOption::NoWrap);
    //QTimer *timer = new QTimer(this);
    //connect(timer,SIGNAL(timeout()),this,SLOT(printScroll()));
    //timer->start(1000);
    top = -1;
    scrollBar = verticalScrollBar();
}

//void SourceEdit::scrollContentsBy ( int dx, int dy )
//{
    //QPlainTextEdit::scrollContentsBy(dx,dy);
    //emit newHeight(heightInPixels);
//}
void SourceEdit::wheelEvent ( QWheelEvent *event )
{
    QPlainTextEdit::wheelEvent(event);
    int x = scrollBar->value();
    if ( x != top ) {
        emit newHeight(heightInPixels);
        top = x;
    }
}

bool SourceEdit::event ( QEvent *event )
{
    bool ret = QPlainTextEdit::event(event);
    int x = scrollBar->value();
    if ( x != top ) {
        emit newHeight(heightInPixels);
        top = x;
    }
    return ret;
}

void SourceEdit::keyPressEvent ( QKeyEvent *event )
{
    //if ( event->matches(QKeySequence::ZoomIn) ) event->ignore();
    QPlainTextEdit::keyPressEvent(event);
}

void SourceEdit::printScroll()
{
    QScrollBar *sb = verticalScrollBar();
    qDebug() << "sb" << sb->minimum() << sb->value() << sb->maximum() << endl;
    qDebug() << viewport()->size();
    qDebug() << "blocks" << blockCount();
}

void SourceEdit::resizeEvent(QResizeEvent *e)
{
    heightInPixels = e->size().height();
    //qDebug() << "Size " << e->size();
    //qDebug() << "ht " << heightInPixels;
    emit newHeight(heightInPixels);
}

SourceWindow::SourceWindow(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(0);

    lineNumberEdit = new LineNumberEdit(this);
    createTextEdit();

    //connect ( quitButton, SIGNAL(clicked()), parent, SLOT(quit()) );

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(5,5,5,5);

    layout->addWidget(lineNumberEdit,0);
    layout->addWidget(textEdit,1);

    setLayout(layout);

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
    if ( fontHeight > 0 ) textHeight = heightInPixels / fontHeight;
    else textHeight = 10;
    setLineNumbers(textDoc->lineCount());
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
