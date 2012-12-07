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

void SourceEdit::scrollContentsBy ( int dx, int dy )
{
    QPlainTextEdit::scrollContentsBy(dx,dy);
    emit newHeight(heightInPixels);
}

//void SourceEdit::wheelEvent ( QWheelEvent *event )
//{
    //QPlainTextEdit::wheelEvent(event);
    //int x = scrollBar->value();
    //if ( x != top ) {
        //emit newHeight(heightInPixels);
        //top = x;
    //}
//}

//bool SourceEdit::event ( QEvent *event )
//{
    //bool ret = QPlainTextEdit::event(event);
    //int x = scrollBar->value();
    //if ( x != top ) {
        //emit newHeight(heightInPixels);
        //top = x;
    //}
    //return ret;
//}

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
    opened = false;
    saved = false;
    lastLineNumber = 0;

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

//void SourceWindow::scrollBarChanged ( int value )
//{
   //setLineNumbers(textDoc->lineCount());
   //topNumber = scrollBar->value() + 1;
//}

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

void SourceWindow::open(QString name)
{
    QFile file(name);

    if (! file.open(QIODevice::ReadWrite))
    {
        if ( ! file.open(QIODevice::ReadOnly) ) {
            QMessageBox::critical(this, tr("Error"),
                tr("Failed to open file ") + name );
            delete this;
            return;
        }
    }

    fileName = name;

    QByteArray text = file.readAll();
    int length = text.count();
    if ( text[length-1] == '\n' ) text.chop(1);
    textEdit->setPlainText(text);

    file.close();
    opened = true;
    changed = false;
}

void SourceWindow::open()
{
    // How shall we set status bar text here?

    opened = false;

    // TODO: Add Fortran file extensions and other assembler extensions
    // Any files?
    QString name = QFileDialog::getOpenFileName(this, tr("Open File"),
        ".", tr("Assembly files (*.asm *akefile *);;") +
             tr("C/C++ files (*.c *.cpp *.h *.hpp *akefile);;") +
             tr("Fortran files (*.f *.F *.f* *.F* *akefile);;All files (*.* *)"));

    if (name == "")
    {
        delete this;
        return;
    }

    QFile file(name);

    if (! file.open(QIODevice::ReadWrite))
    {
        if ( ! file.open(QIODevice::ReadOnly) ) {
            QMessageBox::critical(this, tr("Error"),
                tr("Failed to open file ") + name );
            delete this;
            return;
        }
    }

    fileName = name;

    QByteArray text = file.readAll();
    int length = text.count();
    if ( text[length-1] == '\n' ) text.chop(1);
    textEdit->setPlainText(text);

    file.close();
    opened = true;
    changed = false;
}

void SourceWindow::saveAs()
{
    QString name = QFileDialog::getSaveFileName(this, tr("Save file as"), "." );
    saved = false;

    if (name == "")
    {
        return;
    }

    QFile file(name);

    if (! file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, tr("Error"),
                tr("Failed to open file for writing") + name );
        return;
    }

    fileName = name;

    QTextStream stream(&file);
    stream << textEdit->toPlainText()+"\n";
    stream.flush();
    file.close();

    // File changed variable, reset to false
    qDebug() << "Saved it as " << fileName;
    changed = false;
    saved = true;
}

void SourceWindow::save()
{
    QFile file(fileName);
    saved = false;
    if (! file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to open file ") +
                fileName + tr(" for saving."));
        return;
    } else {
        QTextStream stream(&file);
        stream << textEdit->toPlainText()+"\n";
        stream.flush();
        file.close();

        // File changed variable, reset to false
        changed = false;
        saved = true;
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
    char s[10];

    for (int i = lastLineNumber+1; i <= nLines; i++) {
        sprintf(s,"%4d",i);
        lineNumberEdit->appendPlainText(s);
    }
    QTextCursor cursor;
    for (int i = nLines+1; i <= lastLineNumber; i++ ) {
        sprintf(s,"%4d",i);
        cursor = lineNumberEdit->document()->find(s);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
    }

    lastLineNumber = nLines;

    //qDebug() << "SLN" << nLines << topNumber;
    lineNumberEdit->scrollBar->setValue(scrollBar->value());
}

LineNumberEdit::LineNumberEdit(QWidget *parent)
: QPlainTextEdit(parent)
{
    setFixedWidth(60);
    setReadOnly(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollBar = verticalScrollBar();
    setToolTip(tr("Click on a line number to set or reset a breakpoint"));
    breakpoints = &((SourceWindow *)parent)->breakpoints;
    breakFormat.setBackground(QBrush(QColor(ebe["break_bg"].toString())));
    breakFormat.setForeground(QBrush(QColor(ebe["break_fg"].toString())));
}

void LineNumberEdit::mouseReleaseEvent ( QMouseEvent *e )
{
    int l,t,r,b;
    getContentsMargins(&l,&t,&r,&b);
    qDebug() << "margins" << l << t << r << b;
    SourceWindow *p = (SourceWindow *)parent();
    int row = (e->pos().y()-2)/p->fontHeight + p->topNumber;
    int block = cursorForPosition(e->pos()).blockNumber();
    qDebug() << "mre" << row;
    qDebug() << "block" << block;
    if ( breakpoints->contains(block+1) ) {
        breakpoints->remove(block+1);
        cursorForPosition(e->pos()).setBlockFormat(normalFormat);
    } else {
        breakpoints->insert(block+1);
        cursorForPosition(e->pos()).setBlockFormat(breakFormat);
    }
    //foreach ( int line, *breakpoints ) {
        //qDebug() << "bp at" << line;
    //}
}

void LineNumberEdit::wheelEvent ( QWheelEvent *e )
{
}
