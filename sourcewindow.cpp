#include "sourcewindow.h"
#include "datawindow.h"
#include "stylesheet.h"
#include "settings.h"
#include <QtGui>

extern DataWindow *dataWindow;

extern QList<QString> cppExts;
extern QList<QString> cExts;
extern QList<QString> fortranExts;
extern QList<QString> asmExts;

SourceEdit::SourceEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    setWordWrapMode(QTextOption::NoWrap);
    //QTimer *timer = new QTimer(this);
    //connect(timer,SIGNAL(timeout()),this,SLOT(printScroll()));
    //timer->start(1000);
    top = -1;
    scrollBar = verticalScrollBar();
    tab_width = ebe["tab_width"].toInt();
}

void SourceEdit::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    //qDebug() << "key" << key << event->modifiers();
    if ( key == 46 && event->modifiers() & Qt::ControlModifier ) {
        SourceWindow *p = (SourceWindow *)parent();
        p->indent();
    } else if ( key == 44 && event->modifiers() & Qt::ControlModifier ) {
        SourceWindow *p = (SourceWindow *)parent();
        p->unIndent();
    } else if ( key == Qt::Key_Home && event->modifiers() & Qt::ControlModifier ) {
        SourceWindow *p = (SourceWindow *)parent();
        p->gotoFirstLine();
    } else if ( key == Qt::Key_End && event->modifiers() & Qt::ControlModifier ) {
        SourceWindow *p = (SourceWindow *)parent();
        p->gotoLastLine();
    } else {
        QPlainTextEdit::keyPressEvent(event);
    }
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

void SourceEdit::printScroll()
{
    //QScrollBar *sb = verticalScrollBar();
    //qDebug() << "sb" << sb->minimum() << sb->value() << sb->maximum() << endl;
    //qDebug() << viewport()->size();
    //qDebug() << "blocks" << blockCount();
}

void SourceEdit::contextMenuEvent ( QContextMenuEvent * /* event */ )
{
    QMenu menu("Edit menu");
    menu.addAction(tr("Undo"), this, SLOT(undo()) );
    menu.addAction(tr("Redo"), this, SLOT(redo()) );
    menu.addSeparator();
    menu.addAction(tr("Cut"), this, SLOT(cut()) );
    menu.addAction(tr("Copy"), this, SLOT(copy()) );
    menu.addAction(tr("Paste"), this, SLOT(paste()) );
    menu.addSeparator();
    menu.addAction(tr("Define variable"), this, SLOT(defineVariable()) );
    menu.exec(QCursor::pos());
}

void SourceEdit::defineVariable()
{
    QString text = textCursor().selectedText();
    if ( text.length() == 0 ) return;
    DefineVariableDialog *dialog = new DefineVariableDialog;
    dialog->nameEdit->setText(text);
    dialog->addressEdit->setText("&"+text);
    if ( dialog->exec() ) emit sendVariableDefinition(dialog->result);
    delete dialog;
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

    tab_width = ebe["tab_width"].toInt();

    breakpoints = new IntSet;

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
    breakFormat.setBackground(QBrush(QColor(ebe["next_bg"].toString())));
    breakFormat.setForeground(QBrush(QColor(ebe["next_fg"].toString())));

    scrollBar = textEdit->verticalScrollBar();
    textDoc = textEdit->document();

    // Auto update line number edit control when text changes in main
    // text edit widget
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
    connect ( textEdit, SIGNAL(newHeight(int)), this, SLOT(newHeight(int)));
    connect ( textEdit, SIGNAL(sendVariableDefinition(QStringList)),
              dataWindow, SLOT(receiveVariableDefinition(QStringList)) );
    connect ( scrollBar, SIGNAL(sliderMoved(int)),
              this, SLOT(scrollBarChanged(int)));
    connect ( scrollBar, SIGNAL(valueChanged(int)),
              this, SLOT(scrollBarChanged(int)));
    connect ( scrollBar, SIGNAL(rangeChanged(int,int)),
              this, SLOT(scrollBarRangeChanged(int,int)));
}

void SourceWindow::setFontHeightAndWidth ( int height, int width )
{
    fontHeight = height;
    fontWidth  = width;
    lineNumberEdit->setFixedWidth(width*4+15);
    if ( heightInPixels > 0 ) {
        textHeight = heightInPixels/fontHeight;
    } else {
        textHeight = 0;
    }
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::scrollBarRangeChanged ( int min, int max )
{
    lineNumberEdit->scrollBar->setRange(min,max);
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::scrollBarChanged ( int value )
{
    topNumber = scrollBar->value() + 1;
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::sliderChanged ( int value )
{
    topNumber = scrollBar->value() + 1;
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
    int i = 0;
    int column = 0;
    int next;
    int n;
    int j;
    while ( i < text.count() ) {
        if ( text[i] == '\t' ) {
            next = (column + tab_width)/tab_width*tab_width;
            n = next - column;
            text[i] = ' ';
            for ( j = 1; j < n; j++ ) text.insert(i," ");
        } else if ( text[i] == '\n' ) {
            column = 0;
        } else {
            column++;
        }
        i++;
    }
    textEdit->setPlainText(text);

    file.close();
    opened = true;
    changed = false;
}

void SourceWindow::open()
{
    // How shall we set status bar text here?

    //qDebug() << "sw open";
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
    int i = 0;
    int column = 0;
    int next;
    int n;
    int j;
    while ( i < text.count() ) {
        if ( text[i] == '\t' ) {
            next = (column + tab_width)/tab_width*tab_width;
            n = next - column;
            text[i] = ' ';
            for ( j = 1; j < n; j++ ) text.insert(i," ");
        } else if ( text[i] == '\n' ) {
            column = 0;
        } else {
            column++;
        }
        i++;
    }
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
    //qDebug() << "Saved it as " << fileName;
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

void SourceWindow::setNextLine(int line)
{
    QTextCursor(textEdit->document()->findBlockByNumber(line-1)).setBlockFormat(breakFormat);
    scrollBar->setValue(line-1-textHeight/2);
}

void SourceWindow::clearNextLine(int line)
{
    QTextCursor(textEdit->document()->findBlockByNumber(line-1)).setBlockFormat(normalFormat);
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
    breakpoints = ((SourceWindow *)parent)->breakpoints;
    breakFormat.setBackground(QBrush(QColor(ebe["break_bg"].toString())));
    breakFormat.setForeground(QBrush(QColor(ebe["break_fg"].toString())));
}

void SourceWindow::comment()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextDocument *doc = textEdit->document();
    if ( !cursor.hasSelection() ) return;
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    QTextBlock block;
    block = doc->findBlock(start);
    int startBlock = block.blockNumber();
    block = doc->findBlock(end);
    int endBlock = block.blockNumber();
    int n = fileName.lastIndexOf('.');
    if ( n < 0 ) {
        qDebug() << "File does not have an extension.";
        return;
    }
    QString ext = fileName.mid(n+1);
    int pos;
    if ( cppExts.contains(ext) ) {
        for ( int i = startBlock; i <= endBlock; i++ ) {
            block = doc->findBlockByNumber(i);
            pos = block.position();
            cursor.setPosition(pos);
            cursor.insertText("//");
        }
    }

}

void SourceWindow::unComment()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextDocument *doc = textEdit->document();
    if ( !cursor.hasSelection() ) return;
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    QTextBlock block;
    block = doc->findBlock(start);
    int startBlock = block.blockNumber();
    block = doc->findBlock(end);
    int endBlock = block.blockNumber();
    int n = fileName.lastIndexOf('.');
    if ( n < 0 ) {
        qDebug() << "File does not have an extension.";
        return;
    }
    QString ext = fileName.mid(n+1);
    int pos;
    QString t;
    if ( cppExts.contains(ext) ) {
        for ( int i = startBlock; i <= endBlock; i++ ) {
            block = doc->findBlockByNumber(i);
            pos = block.position();
            cursor.setPosition(pos);
            cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,2);
            t = cursor.selectedText();
            if ( t == "//" ) cursor.deleteChar();
        }
        cursor.setPosition(start);
        cursor.setPosition(end,QTextCursor::KeepAnchor);
    }

}

void SourceWindow::indent()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextDocument *doc = textEdit->document();
    if ( !cursor.hasSelection() ) return;
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    QTextBlock block;
    block = doc->findBlock(start);
    int startBlock = block.blockNumber();
    block = doc->findBlock(end);
    int endBlock = block.blockNumber();
    int n = fileName.lastIndexOf('.');
    if ( n < 0 ) {
        qDebug() << "File does not have an extension.";
        return;
    }
    QString ext = fileName.mid(n+1);
    int pos;
    QString prefix;
    for ( int i=0; i < tab_width; i++ ) prefix += " ";
    if ( cppExts.contains(ext) ) {
        for ( int i = startBlock; i <= endBlock; i++ ) {
            block = doc->findBlockByNumber(i);
            pos = block.position();
            cursor.setPosition(pos);
            cursor.insertText(prefix);
        }
    }

}

void SourceWindow::unIndent()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextDocument *doc = textEdit->document();
    if ( !cursor.hasSelection() ) return;
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    QTextBlock block;
    block = doc->findBlock(start);
    int startBlock = block.blockNumber();
    block = doc->findBlock(end);
    int endBlock = block.blockNumber();
    int n = fileName.lastIndexOf('.');
    if ( n < 0 ) {
        qDebug() << "File does not have an extension.";
        return;
    }
    QString ext = fileName.mid(n+1);
    int pos;
    QString t;
    QString prefix;
    for ( int i=0; i < tab_width; i++ ) prefix += " ";
    if ( cppExts.contains(ext) ) {
        for ( int i = startBlock; i <= endBlock; i++ ) {
            block = doc->findBlockByNumber(i);
            pos = block.position();
            cursor.setPosition(pos);
            cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,tab_width);
            t = cursor.selectedText();
            if ( t == prefix ) cursor.deleteChar();
        }
        cursor.setPosition(start);
        cursor.setPosition(end,QTextCursor::KeepAnchor);
    }

}

void SourceWindow::find()
{
    FindReplaceDialog *dialog = new FindReplaceDialog;
    dialog->textEdit = textEdit;
    dialog->exec();
}

void SourceWindow::pageForward()
{
    QTextDocument *doc = textEdit->document();
    QTextCursor cursor = textEdit->textCursor();
    int pos = cursor.position();
    QTextBlock block;
    block = doc->findBlock(pos);
    int blockNumber = block.blockNumber();
    if ( blockNumber + textHeight >= doc->lineCount() ) {
        blockNumber = doc->lineCount() - 1;
    } else {
        blockNumber += textHeight;
    }
    block = doc->findBlockByNumber(blockNumber);
    pos = block.position();
    cursor.setPosition(pos);
    textEdit->setTextCursor(cursor);
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::pageBackward()
{
    QTextDocument *doc = textEdit->document();
    QTextCursor cursor = textEdit->textCursor();
    int pos = cursor.position();
    QTextBlock block;
    block = doc->findBlock(pos);
    int blockNumber = block.blockNumber();
    if ( blockNumber - textHeight < 0 ) {
        blockNumber = 0;
    } else {
        blockNumber -= textHeight;
    }
    block = doc->findBlockByNumber(blockNumber);
    pos = block.position();
    cursor.setPosition(pos);
    textEdit->setTextCursor(cursor);
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::center()
{
    QTextDocument *doc = textEdit->document();
    QTextCursor cursor = textEdit->textCursor();
    int pos = cursor.position();
    QTextBlock block;
    block = doc->findBlock(pos);
    int blockNumber = block.blockNumber();
    int top;
    if ( blockNumber > textHeight/2 ) {
        top = blockNumber - textHeight/2;
    } else {
        top = 0;
    }
    scrollBar->setValue(top);
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::gotoFirstLine()
{
    QTextCursor cursor = textEdit->textCursor();
    cursor.setPosition(0);
    textEdit->setTextCursor(cursor);
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::gotoLastLine()
{
    QTextDocument *doc = textEdit->document();
    QTextCursor cursor = textEdit->textCursor();
    int blockNumber = doc->lineCount()-1;
    QTextBlock block = doc->findBlockByNumber(blockNumber);
    int pos = block.position();
    cursor.setPosition(pos);
    textEdit->setTextCursor(cursor);
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::gotoTop()
{
    QTextDocument *doc = textEdit->document();
    QTextCursor cursor = textEdit->textCursor();
    int blockNumber = scrollBar->value();
    QTextBlock block;
    block = doc->findBlockByNumber(blockNumber);
    int pos = block.position();
    cursor.setPosition(pos);
    textEdit->setTextCursor(cursor);
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::gotoBottom()
{
    QTextDocument *doc = textEdit->document();
    QTextCursor cursor = textEdit->textCursor();
    int blockNumber = scrollBar->value()+textHeight-2;
    QTextBlock block;
    block = doc->findBlockByNumber(blockNumber);
    int pos = block.position();
    cursor.setPosition(pos);
    textEdit->setTextCursor(cursor);
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::gotoLine()
{
    qDebug() << "gotoLine";
    QTextDocument *doc = textEdit->document();
    LineNumberDialog *dialog = new LineNumberDialog;
    QTextCursor cursor = textEdit->textCursor();
    int pos = cursor.position();
    QTextBlock block;
    block = doc->findBlock(pos);
    dialog->line = block.blockNumber()+1;
    dialog->setMax(doc->lineCount());
    if ( dialog->exec() ) {
        int blockNumber = dialog->line - 1;
        block = doc->findBlockByNumber(blockNumber);
        int pos = block.position();
        cursor.setPosition(pos);
        textEdit->setTextCursor(cursor);
        setLineNumbers(textDoc->lineCount());
    }
    delete dialog;
    center();
}

void SourceWindow::prettify()
{
    save();
    QProcess indent(this);
    QString cmd = ebe["prettify"].toString();
    cmd.replace("$source",fileName);
    indent.start(cmd);
    indent.waitForFinished();
    open(fileName);
}

void LineNumberEdit::mouseReleaseEvent ( QMouseEvent *e )
{
    //SourceWindow *p = (SourceWindow *)parent();
    //int row = (e->pos().y()-2)/p->fontHeight + p->topNumber;
    int block = cursorForPosition(e->pos()).blockNumber();
    //qDebug() << "mre" << row;
    //qDebug() << "block" << block;
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

void LineNumberEdit::contextMenuEvent(QContextMenuEvent *event)
{
    eventPosition = event->pos();
    QMenu menu("Breakpoint menu");
    menu.addAction(tr("Set breakpoint"),
                   this, SLOT(setBreakpoint()) );
    menu.addAction(tr("Drop breakpoint"),
                   this, SLOT(dropBreakpoint()) );
    menu.addAction(tr("Drop all breakpoints"),
                   this, SLOT(dropAllBreakpoints()) );
    menu.addAction(tr("ignore"), this, SLOT(ignore()) );
    menu.exec(QCursor::pos());
}

void LineNumberEdit::ignore()
{
}

void LineNumberEdit::dropAllBreakpoints()
{
    //SourceWindow *p = (SourceWindow *)parent();
    //qDebug() << "top" << p->topNumber;
    foreach ( int line, *breakpoints ) {
        //qDebug() << "bp at" << line;
        breakpoints->remove(line);
        //eventPosition.setX(0);
        //eventPosition.setY((line-p->topNumber)*p->fontHeight+p->fontHeight/2+1);
        //qDebug() << "Pos" << eventPosition;
        QTextCursor(document()->findBlockByNumber(line-1)).setBlockFormat(normalFormat);
    }
}

void LineNumberEdit::setBreakpoint()
{
    //SourceWindow *p = (SourceWindow *)parent();
    //int row = (eventPosition.y()-2)/p->fontHeight + p->topNumber;
    int block = cursorForPosition(eventPosition).blockNumber();
    //qDebug() << "set" << row;
    //qDebug() << "block" << block;
    breakpoints->insert(block+1);
    cursorForPosition(eventPosition).setBlockFormat(breakFormat);
}

void LineNumberEdit::dropBreakpoint()
{
    //qDebug() << "drop" << eventPosition;
    //SourceWindow *p = (SourceWindow *)parent();
    //int row = (eventPosition.y()-2)/p->fontHeight + p->topNumber;
    int block = cursorForPosition(eventPosition).blockNumber();
    //qDebug() << "set" << row;
    //qDebug() << "block" << block;
    breakpoints->remove(block+1);
    cursorForPosition(eventPosition).setBlockFormat(normalFormat);
}

void LineNumberEdit::wheelEvent ( QWheelEvent * /* e */ )
{
}

LineNumberDialog::LineNumberDialog()
: QDialog()
{
    setObjectName("Go to line");
    setWindowTitle("Go to line");
    setModal(true);
    setWindowFlags(Qt::Dialog|Qt::WindowStaysOnTopHint);

    move(QCursor::pos());
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10,10,10,10);

    QHBoxLayout *lineLayout = new QHBoxLayout;
    lineLayout->addWidget ( new QLabel(tr("Line number")) );
    lineSpin = new QSpinBox;
    lineSpin->setMinimum(1);
    lineLayout->addWidget ( lineSpin );
    layout->addLayout(lineLayout);

    okButton = new QPushButton("Go");
    cancelButton = new QPushButton("Cancel");
    connect(okButton, SIGNAL(clicked()), this, SLOT(setLine()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    setLayout(layout);
}

void LineNumberDialog::setLine()
{
    line = lineSpin->value();
    accept();
}

void LineNumberDialog::setMax(int max)
{
    lineSpin->setMaximum(max);
}

FindReplaceDialog::FindReplaceDialog()
: QDialog()
{
    setObjectName("Find/Replace");
    setWindowTitle("Find/Replace");
    setModal(false);
    setWindowFlags(Qt::Dialog|Qt::WindowStaysOnTopHint);

    move(QCursor::pos());
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10,10,10,10);

    QHBoxLayout *findLayout = new QHBoxLayout;
    findLayout->addWidget ( new QLabel(tr("Find")) );
    findEdit = new QLineEdit;
    findLayout->addWidget ( findEdit );
    layout->addLayout(findLayout);

    QHBoxLayout *replaceLayout = new QHBoxLayout;
    replaceLayout->addWidget ( new QLabel(tr("Replace")) );
    replaceEdit = new QLineEdit;
    replaceLayout->addWidget ( replaceEdit );
    layout->addLayout(replaceLayout);

    findButton = new QPushButton("Find");
    replaceButton = new QPushButton("Replace");
    cancelButton = new QPushButton("Cancel");
    connect(findButton, SIGNAL(clicked()), this, SLOT(find()));
    connect(replaceButton, SIGNAL(clicked()), this, SLOT(replace()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    buttonLayout->addStretch();
    buttonLayout->addWidget(findButton);
    buttonLayout->addWidget(replaceButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    layout->addLayout(buttonLayout);

    setLayout(layout);
}

void FindReplaceDialog::find()
{
    if ( !textEdit->find(findEdit->text()) ) {
        textEdit->moveCursor(QTextCursor::Start);
        textEdit->find(findEdit->text());
    }
}

void FindReplaceDialog::replace()
{
    textEdit->insertPlainText(replaceEdit->text());
}

void FindReplaceDialog::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    //qDebug() << "key" << key << event->modifiers();
    if ( key == 'F' && event->modifiers() & Qt::ControlModifier ) {
        find();
    } else if ( key == 'R' && event->modifiers() & Qt::ControlModifier ) {
        replace();
    } else {
        QDialog::keyPressEvent(event);
    }
}

