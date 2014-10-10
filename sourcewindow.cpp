#include "sourcewindow.h"
#include "sourceframe.h"
#include "datawindow.h"
#include "projectwindow.h"
#include "gdb.h"
#include "stylesheet.h"
#include "settings.h"
#include <QtGui>
#include <QStatusBar>
#include <QScrollBar>
#include <QMessageBox>
#include <QMenu>
#include <QFileDialog>

extern DataWindow *dataWindow;
extern ProjectWindow *projectWindow;
extern SourceFrame *sourceFrame;
extern GDB *gdb;

extern QStringList cppExts;
extern QStringList cExts;
extern QStringList fortranExts;
extern QStringList halExts;
extern QStringList asmExts;

extern QStatusBar *statusBar;

SourceEdit::SourceEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    setWordWrapMode(QTextOption::NoWrap);
    //QTimer *timer = new QTimer(this);
    //connect(timer,SIGNAL(timeout()),this,SLOT(printScroll()));
    //timer->start(1000);
    setToolTip(tr("Right click to popup a menu.\n"
        "Mark a variable and right click to define\n"
        "a user-defined variable. This is most\n"
        "useful for assembly language."));
    top = -1;
    scrollBar = verticalScrollBar();
    tab_width = ebe["edit/tab_width"].toInt();
    autoIndent = ebe["edit/auto_indent"].toBool();
    c = 0;
    QCompleter *completer = new QCompleter(this);
    setCompleter(completer);
    c->setModel(&model);
    QStringList list;
    complete_minimum = ebe["complete/minimum"].toInt();
    list << "include" << "using" << "namespace" << "class" << "struct"
        << "union" << "double" << "extern" << "static" << "volatile"
        << "parameter" << "variable" << "allocate" << "assert" << "asynchronous"
        << "average" << "binary" << "break" << "catch" << "const" << "close"
        << "const_cast" << "critical" << "continue" << "cout" << "bool"
        << "dynamic_cast" << "decimal" << "define" << "dimension" << "delete"
        << "default" << "error" << "extern" << "external" << "fclose" << "first"
        << "float" << "flush" << "fflush" << "fopen" << "format" << "fprintf"
        << "fscanf" << "fstream" << "false" << "generic" << "goto" << "height"
        << "hello world!" << "Hello world!" << "hexadecimal" << "ifdef"
        << "ifndef" << "implicit" << "import" << "index" << "inline"
        << "INT_MIN" << "INT_MAX" << "integer" << "intrinsic" << "iostream"
        << "iomanip" << "iterator" << "last" << "length" << "lock" << "long"
        << "lowercase" << "macro" << "MAX_RAND" << "memory" << "middle"
        << "module" << "main" << "negative" << "npos" << "NULL" << "only"
        << "open" << "operator" << "optional" << "people" << "person"
        << "pointer" << "polynomial" << "positive" << "precision" << "prefix"
        << "print" << "printf" << "private" << "protected" << "public" << "pure"
        << "push_back" << "queue" << "random" << "read" << "recursive"
        << "register" << "reinterpret_cast" << "result" << "reverse" << "rewind"
        << "return" << "real" << "save" << "scanf" << "section" << "segment"
        << "select" << "sequence" << "short" << "signed" << "square"
        << "static_cast" << "string" << "strlen" << "student" << "substr"
        << "suffix" << "switch" << "sync" << "static" << "sizeof" << "typeid"
        << "target" << "text" << "this" << "throw" << "total" << "true"
        << "typedef" << "undef" << "unlock" << "unsigned" << "uppercase"
        << "value" << "virtual" << "vector" << "void" << "wait" << "wchar_t"
        << "weight" << "write" << "while" << "stdio.h" << "stdlib.h" << "cstdio"
        << "math.h";
    list.sort();

    list = list.filter(QRegExp(QString(".{%1,}").arg(complete_minimum)));
    model.setStringList(list);
    c->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    c->setCaseSensitivity(Qt::CaseSensitive);
    c->setWrapAround(false);

    highlighter = new CppHighlighter(document());

    connect(this, SIGNAL(cursorPositionChanged()), sourceFrame,
        SLOT(updateCursorPosition()));
}

void SourceEdit::setCompleter(QCompleter *completer)
{
    if (c) disconnect(c, 0, this, 0);

    c = completer;

    if (!c) return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseSensitive);
    QObject::connect(c, SIGNAL(activated(QString)), this,
        SLOT(insertCompletion(QString)));
}

QCompleter *SourceEdit::completer() const
{
    return c;
}

void SourceEdit::insertCompletion(const QString& completion)
{
    if (c->widget() != this) return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString SourceEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void SourceEdit::focusInEvent(QFocusEvent *e)
{
    if (c) c->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

void SourceEdit::addWords(QString t)
{
    QRegExp rx(QString("([a-zA-Z][a-zA-Z0-9_]{%1,})").arg(complete_minimum));
    QString word;

    int i = 0;
    while (true) {
        i = rx.indexIn(t, i);
        if (i < 0) break;
        word = rx.cap(1);
        if (!wordsInList.contains(word)) {
            model.insertRow(0);
            model.setData(model.index(0), word);
            wordsInList.insert(word);
        }
        i += word.length();
    }
    model.sort(0);
}

QString lastPrefix = "";

void SourceEdit::keyPressEvent(QKeyEvent *e)
{
    static QString eow("\r ~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word

    static int lastKey = 0;
    int key = e->key();
    if (c && c->popup()->isVisible()) {
        switch (key) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
            //case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
        default:
            break;
        }
    }

    int mod = e->modifiers();
    //qDebug() << "key" << key << mod << e->text() << int(e->text()[0].toAscii());
    if (key == Qt::Key_BraceLeft && mod == Qt::ShiftModifier) {
        //qDebug() << "BraceLeft";
        lastKey = key;
        QTextCursor cursor = textCursor();
        cursor.insertText("{");
    } else if (key == Qt::Key_BraceRight && mod == Qt::ShiftModifier) {
        //qDebug() << "BraceRight";
        lastKey = key;
        QTextCursor cursor = textCursor();
        if (autoIndent) {
            QTextBlock block = cursor.block();
            int col = cursor.position() - block.position();
            QString t = block.text();
            int prev = (col - 1) / tab_width * tab_width;
            //qDebug() << "prev" << prev << col;
            for (int i = col - 1; i >= prev; i--) {
                //qDebug() << "i" << i << t[i];
                if (t[i] == ' ') {
                    cursor.deletePreviousChar();
                } else {
                    break;
                }
            }
        }
        cursor.insertText("}");
    } else if (key == Qt::Key_Return) {
        if (lastPrefix.length() >= complete_minimum
            && !wordsInList.contains(lastPrefix)) {
            model.insertRow(0);
            model.setData(model.index(0), lastPrefix);
            wordsInList.insert(lastPrefix);
            model.sort(0);
        }
        int k = 0;
        if (lastKey == Qt::Key_BraceLeft) k = 1;
        if (autoIndent && mod == 0)
            indentNewLine(k);
        else
            newLine();
        lastKey = 0;
    } else if ((key == Qt::Key_Period || key == Qt::Key_Less)
        && (mod & Qt::ControlModifier)) {
        lastKey = 0;
        SourceWindow *p = (SourceWindow *)parent();
        p->indent();
    } else if ((key == Qt::Key_Comma || key == Qt::Key_Greater)
        && (mod & Qt::ControlModifier)) {
        lastKey = 0;
        SourceWindow *p = (SourceWindow *)parent();
        p->unIndent();
    } else if (key == Qt::Key_Home && (mod & Qt::ControlModifier)) {
        lastKey = 0;
        SourceWindow *p = (SourceWindow *)parent();
        p->gotoFirstLine();
    } else if (key == Qt::Key_End && (mod & Qt::ControlModifier)) {
        lastKey = 0;
        SourceWindow *p = (SourceWindow *)parent();
        p->gotoLastLine();
    } else if (key == Qt::Key_Tab && mod == 0) {
        lastKey = 0;
        QTextCursor cursor = textCursor();
        QTextBlock block = cursor.block();
        int col = cursor.position() - block.position();
        int next = (col + tab_width) / tab_width * tab_width;
        int n = next - col;
        for (int i = 0; i < n; i++)
            cursor.insertText(" ");
    } else if (key == Qt::Key_Tab && (mod & Qt::ControlModifier)) {
        lastKey = 0;
        QTextCursor cursor = textCursor();
        QTextBlock block = cursor.block();
        int col = cursor.position() - block.position();
        QString t = block.text();
        int prev = (col - 1) / tab_width * tab_width;
        //qDebug() << "prev" << prev << col;
        for (int i = col - 1; i >= prev; i--) {
            //qDebug() << "i" << i << t[i];
            if (t[i] == ' ') {
                cursor.deletePreviousChar();
            } else {
                break;
            }
        }
    } else {
        QPlainTextEdit::keyPressEvent(e);
        const bool ctrlOrShift = mod
            & (Qt::ControlModifier | Qt::ShiftModifier);
        if (!c || (ctrlOrShift && e->text().isEmpty())) return;

        bool hasModifier = (mod != Qt::NoModifier) && !ctrlOrShift;
        QString completionPrefix = textUnderCursor();

        if (hasModifier || e->text().isEmpty() || completionPrefix.length() < 1
            || eow.contains(e->text().right(1))) {
            c->popup()->hide();
            if (lastPrefix.length() >= complete_minimum
                && !wordsInList.contains(lastPrefix)) {
                model.insertRow(0);
                model.setData(model.index(0), lastPrefix);
                wordsInList.insert(lastPrefix);
                model.sort(0);
            }
            return;
        }

        lastPrefix = completionPrefix;

        if (completionPrefix != c->completionPrefix()) {
            c->setCompletionPrefix(completionPrefix);
            c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
        }
        QRect cr = cursorRect();
        cr.setWidth(
            c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
        c->complete(cr); // popup it up!
    }
}

void SourceEdit::indentNewLine(int x)
{
    //qDebug() << "inl";
    tab_width = ebe["edit/tab_width"].toInt();
    QTextCursor cursor = textCursor();
    QTextBlock block = cursor.block();
    QString t = block.text();
    int n = t.length();
    int k = 0;
    cursor.insertText("\n");
    cursor = textCursor();
    while (k < n && t[k] == ' ') {
        k++;
    }
    k += x * tab_width;
    for (int i = 0; i < k; i++) {
        cursor.insertText(" ");
    }
}

void SourceEdit::newLine()
{
    //qDebug() << "nl";
    textCursor().insertText("\n");
}

void SourceEdit::scrollContentsBy(int dx, int dy)
{
    QPlainTextEdit::scrollContentsBy(dx, dy);
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

void SourceEdit::contextMenuEvent(QContextMenuEvent * /* event */)
{
    QMenu menu("Edit menu");
    menu.addAction(tr("Undo"), this, SLOT(undo()));
    menu.addAction(tr("Redo"), this, SLOT(redo()));
    menu.addSeparator();
    menu.addAction(tr("Cut"), this, SLOT(cut()));
    menu.addAction(tr("Copy"), this, SLOT(copy()));
    menu.addAction(tr("Paste"), this, SLOT(paste()));
    if (gdb->running) {
        menu.addSeparator();
        menu.addAction(tr("Define variable"), this, SLOT(defineVariable()));
    }
    menu.exec(QCursor::pos());
}

void SourceEdit::defineVariable()
{
    QString text = textCursor().selectedText();
    if (text.length() == 0) return;
    DefineVariableDialog *dialog = new DefineVariableDialog;
    dialog->nameEdit->setText(text);
    dialog->addressEdit->setText("&" + text);
    if (dialog->exec()) {
        SourceWindow *p = (SourceWindow *)parent();
        dialog->result.append(p->file.source);
        int pos = textCursor().position();
        QTextBlock block;
        block = document()->findBlock(pos);
        int line = block.blockNumber() + 1;
        dialog->result.append(QString("%1").arg(line));
        emit sendVariableDefinition(dialog->result);
    }
    delete dialog;
}

void SourceEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    heightInPixels = e->size().height();
    //qDebug() << "Size " << e->size();
    //qDebug() << "ht " << heightInPixels;
    emit newHeight(heightInPixels);
}

SourceWindow::SourceWindow(QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(0);

    tab_width = ebe["edit/tab_width"].toInt();

    breakpoints = new IntSet;

    lineNumberEdit = new LineNumberEdit(this);
    createTextEdit();

    //connect ( quitButton, SIGNAL(clicked()), parent, SLOT(quit()) );

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(5, 5, 5, 5);

    layout->addWidget(lineNumberEdit, 0);
    layout->addWidget(textEdit, 1);

    setLayout(layout);

    heightInPixels = 0;
    textHeight = 0;
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
    connect(textEdit, SIGNAL(sendVariableDefinition(QStringList)), dataWindow,
            SLOT(receiveVariableDefinition(QStringList)));
    connect ( scrollBar, SIGNAL(sliderMoved(int)),
            this, SLOT(scrollBarChanged(int)));
    connect ( scrollBar, SIGNAL(valueChanged(int)),
            this, SLOT(scrollBarChanged(int)));
    connect ( scrollBar, SIGNAL(rangeChanged(int,int)),
            this, SLOT(scrollBarRangeChanged(int,int)));
}

void SourceWindow::clear()
{
    textEdit->clear();
}

void SourceWindow::setFontHeightAndWidth(int height, int width)
{
    fontHeight = height;
    fontWidth = width;
    lineNumberEdit->setFixedWidth(width * 4 + 15);
    if (heightInPixels > 0) {
        textHeight = heightInPixels / fontHeight;
    } else {
        textHeight = 0;
    }
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::scrollBarRangeChanged(int min, int max)
{
    lineNumberEdit->scrollBar->setRange(min, max);
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::scrollBarChanged(int /* value */)
{
    topNumber = scrollBar->value() + 1;
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::sliderChanged(int /* value */)
{
    topNumber = scrollBar->value() + 1;
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::newHeight(int height)
{
    heightInPixels = height;
    if (fontHeight > 0) textHeight = heightInPixels / fontHeight;
    else textHeight = 10;
    //qDebug() << "lines" << textDoc->lineCount();
    //qDebug() << "newheight" << textHeight;
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::doTemplate(QAction *a)
{
    //qDebug() << "iconText" << a->iconText();
    QString name = a->iconText();
    int n = name.indexOf(':');
    if (n > 0) name = name.left(n);
    //qDebug() << name << file.language << ebe.os;

    if (file.language == "asm") {
        QFile in(QString(":/src/%1/%2/%3").arg("assembly").arg(ebe.os).arg(name));
        if (in.open(QFile::ReadOnly)) {
            QString data = QString(in.readAll());
            textEdit->textCursor().insertText(data);
        }
    } else {
        QFile in(QString(":/src/%1/%2").arg(file.language).arg(name));
        if (in.open(QFile::ReadOnly)) {
            QString data = QString(in.readAll());
            textEdit->textCursor().insertText(data);
        }
    }
}

void SourceWindow::insertFile(QString f)
{
    QFile in(f);
    if (in.open(QFile::ReadOnly)) {
        QString data = QString(in.readAll());
        //qDebug() << data;
        textEdit->textCursor().insertText(data);
    }
}

void SourceWindow::createTextEdit()
{
    textEdit = new SourceEdit(this);
    // textEdit->appendPlainText("Text");
}

void SourceWindow::open(QString name)
{
    tab_width = ebe["edit/tab_width"].toInt();
    QFile f(name);

    if (!f.open(QIODevice::ReadWrite)) {
        if (!f.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"),
                    tr("Failed to open file ") + name);
            return;
        }
    }

    file.source = QDir::current().absoluteFilePath(name);
    file.setLanguage();
    ebe["language"] = file.language;
    if (file.language == "fortran") {
        textEdit->highlighter = new FortranHighlighter(textEdit->document());
    } else if (file.language == "asm") {
        textEdit->highlighter = new AsmHighlighter(textEdit->document());
    } else if (file.language == "hal") {
        textEdit->highlighter = new AsmHighlighter(textEdit->document());
    }

    QByteArray text = f.readAll();
    int length = text.count();
    if (length > 0 && text[length - 1] == '\n') text.chop(1);
    int i = 0;
    int column = 0;
    int next;
    int n;
    int j;
    while (i < text.count()) {
        if (text[i] == '\t') {
            next = (column + tab_width) / tab_width * tab_width;
            n = next - column;
            text[i] = ' ';
            for (j = 1; j < n; j++)
                text.insert(i, " ");
            column = next;
            i = next - 1;
        } else if (text[i] == '\n') {
            column = 0;
        } else {
            column++;
        }
        i++;
    }
    textEdit->addWords(QString(text));
    textEdit->setPlainText(text);

    f.close();

    opened = true;
    breakpoints->clear();
    changed = false;
    restoreCursor();
    textEdit->setFocus();
}

void SourceWindow::open()
{
    // How shall we set status bar text here?
    QString patterns;

    opened = false;

    //qDebug() << "language" << ebe["language"];
    QString selected = tr("C/C++ files (*.c* *.h *.t *akefile)");
    if (ebe["language"].toString() == "fortran") {
        selected = tr("Fortran files (*.f* *.F* *akefile)");
        patterns = tr("Fortran files (*.f* *.F* *akefile);;")
            + tr("C/C++ files (*.c* *.h* *.t *akefile);;")
            + tr("Assembly files (*.asm *.s *akefile);;")
            + tr("HAL files (*.hal *akefile);;");
    } else if (ebe["language"].toString() == "asm") {
        //qDebug() << "asm";
        selected = tr("Assembly files (*.asm *.s *akefile)");
        patterns = tr("Assembly files (*.asm *.s *akefile);;")
            + tr("C/C++ files (*.c* *.h* *.t *akefile);;")
            + tr("Fortran files (*.f* *.F* *akefile);;")
            + tr("HAL files (*.hal *akefile);;");
    } else if (ebe["language"].toString() == "hal") {
        //qDebug() << "asm";
        selected = tr("HAL files (*.hal *akefile)");
        patterns = tr("HAL files (*.hal *akefile);;")
            + tr("Assembly files (*.asm *.s *akefile);;")
            + tr("C/C++ files (*.c* *.h* *.t *akefile);;")
            + tr("Fortran files (*.f* *.F* *akefile);;");
    } else {
        patterns = tr("C/C++ files (*.c* *.h* *.t *akefile);;")
            + tr("Fortran files (*.f* *.F* *akefile);;")
            + tr("Assembly files (*.asm *.s *akefile);;")
            + tr("HAL files (*.hal *akefile);;");
    }

    //qDebug() << selected;

    QString name = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
            patterns + tr("All files (*)"), &selected);

    if (name == "") {
        return;
    }

    QFile f(name);

    if (!f.open(QIODevice::ReadWrite)) {
        if (!f.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"),
                    tr("Failed to open file ") + name);
            return;
        }
    }

    file.source = QDir::current().absoluteFilePath(name);

    QByteArray text = f.readAll();
    int length = text.count();
    if (text[length - 1] == '\n') text.chop(1);
    int i = 0;
    int column = 0;
    int next;
    int n;
    int j;
    while (i < text.count()) {
        if (text[i] == '\t') {
            next = (column + tab_width) / tab_width * tab_width;
            n = next - column;
            text[i] = ' ';
            for (j = 1; j < n; j++)
                text.insert(i, " ");
            column = next;
            i = next - 1;
        } else if (text[i] == '\n') {
            column = 0;
        } else {
            column++;
        }
        i++;
    }
    textEdit->addWords(QString(text));
    textEdit->setPlainText(text);
    setFontHeightAndWidth(sourceFrame->fontHeight, sourceFrame->fontWidth);

    f.close();
    projectWindow->addFile(file.source);
    file.setLanguage();
    ebe["language"] = file.language;
    if (file.language == "fortran") {
        textEdit->highlighter = new FortranHighlighter(textEdit->document());
    } else if (file.language == "asm") {
        textEdit->highlighter = new AsmHighlighter(textEdit->document());
    }
    opened = true;
    breakpoints->clear();
    changed = false;
    restoreCursor();
    textEdit->setFocus();
}

void SourceWindow::saveAs()
{
    QString name = QFileDialog::getSaveFileName(this, tr("Save file as"), ".");
    saved = false;

    if (name == "") {
        return;
    }

    QFile f(name);

    if (!f.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"),
                tr("Failed to open file for writing") + name);
        return;
    }

    file.source = QDir::current().absoluteFilePath(name);

    QTextStream stream(&f);
    stream << textEdit->toPlainText() + "\n";
    stream.flush();
    f.close();

    // File changed variable, reset to false
    //qDebug() << "Saved it as " << file.source;
    file.setLanguage();
    if (file.language == "fortran") {
        textEdit->highlighter = new FortranHighlighter(textEdit->document());
    } else if (file.language == "asm") {
        textEdit->highlighter = new AsmHighlighter(textEdit->document());
    }
    changed = false;
    saved = true;
    saveCursor();
}

void SourceWindow::save()
{
    if (file.source == "") {
        saveAs();
        return;
    }
    QFile f(file.source);
    saved = false;
    if (!f.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"),
                tr("Failed to open file ") + file.source + tr(" for saving."));
        return;
    } else {
        QTextStream stream(&f);
        stream << textEdit->toPlainText() + "\n";
        stream.flush();
        f.close();

        // File changed variable, reset to false
        changed = false;
        saved = true;
        saveCursor();
    }
}

void SourceWindow::saveCursor()
{
    ebe[QString("cursor/%1").arg(file.source)] = textEdit->textCursor().position();
}

void SourceWindow::restoreCursor()
{
    QTextCursor cursor = textEdit->textCursor();
    cursor.setPosition(ebe[QString("cursor/%1").arg(file.source)].toInt());
    textEdit->setTextCursor(cursor);
    center();
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
    //char s[10];
    QString s;
    QTextDocument *doc = lineNumberEdit->document();

    for (int i = lastLineNumber + 1; i <= nLines; i++) {
        s.sprintf("%4d", i);
        lineNumberEdit->appendPlainText(s);
        QTextCursor(doc->findBlockByNumber(i - 1)).setBlockFormat(normalFormat);
    }
    QTextCursor cursor;
    for (int i = nLines + 1; i <= lastLineNumber; i++) {
        s.sprintf("%4d", i);
        cursor = doc->find(s);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
    }

    lastLineNumber = nLines;

    //qDebug() << "SLN" << nLines << topNumber;
    lineNumberEdit->scrollBar->setValue(scrollBar->value());
}

void SourceWindow::setNextLine(int line)
{
    //qDebug() << "sw snl" << line;
    QTextDocument *doc = textEdit->document();
    bool saveChanged = changed;
    QTextCursor(doc->findBlockByNumber(line - 1)).setBlockFormat(breakFormat);
    //qDebug() << "scroll set" << line-1-textHeight/2;
    scrollBar->setValue(line - 1 - textHeight / 2);
    setLineNumbers(textDoc->lineCount());
    changed = saveChanged;
}

void SourceWindow::clearNextLine(int line)
{
    bool saveChanged = changed;
    QTextDocument *doc = textEdit->document();
    QTextCursor(doc->findBlockByNumber(line - 1)).setBlockFormat(normalFormat);
    changed = saveChanged;
}

    LineNumberEdit::LineNumberEdit(QWidget *parent)
: QPlainTextEdit(parent)
{
    myParent = (SourceWindow *)parent;
    setFixedWidth(60);
    setReadOnly(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollBar = verticalScrollBar();
    setToolTip(tr("Click on a line number to set or reset a breakpoint"));
    breakpoints = ((SourceWindow *)parent)->breakpoints;
    breakFormat.setBackground(QBrush(QColor(ebe["break_bg"].toString())));
    breakFormat.setForeground(QBrush(QColor(ebe["break_fg"].toString())));
    connect(this, SIGNAL(sendBreakpoint(QString, QString)), gdb,
            SLOT(setBreakpoint(QString, QString)));
    connect(this, SIGNAL(deleteBreakpoint(QString, QString)), gdb,
            SLOT(deleteBreakpoint(QString, QString)));
}

void SourceWindow::comment()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextDocument *doc = textEdit->document();
    int start;
    int end;
    if (cursor.hasSelection()) {
        start = cursor.selectionStart();
        end = cursor.selectionEnd();
    } else {
        start = end = cursor.position();
    }
    QTextBlock block;
    block = doc->findBlock(start);
    int startBlock = block.blockNumber();
    block = doc->findBlock(end);
    int endBlock = block.blockNumber();
    int n = file.source.lastIndexOf('.');
    if (n < 0) {
        qDebug() << "File does not have an extension.";
        return;
    }
    QString ext = file.source.mid(n + 1);
    int pos;
    QString commentMark;
    if (cppExts.contains(ext)) {
        commentMark = "//";
    } else if (fortranExts.contains(ext)) {
        commentMark = "!";
    } else if (asmExts.contains(ext)) {
        commentMark = ";";
    } else {
        return;
    }

    for (int i = startBlock; i <= endBlock; i++) {
        block = doc->findBlockByNumber(i);
        pos = block.position();
        cursor.setPosition(pos);
        cursor.insertText(commentMark);
    }

}

void SourceWindow::unComment()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextDocument *doc = textEdit->document();
    int start;
    int end;
    if (cursor.hasSelection()) {
        start = cursor.selectionStart();
        end = cursor.selectionEnd();
    } else {
        start = end = cursor.position();
    }
    QTextBlock block;
    block = doc->findBlock(start);
    int startBlock = block.blockNumber();
    block = doc->findBlock(end);
    int endBlock = block.blockNumber();
    int n = file.source.lastIndexOf('.');
    if (n < 0) {
        qDebug() << "File does not have an extension.";
        return;
    }
    QString ext = file.source.mid(n + 1);
    int pos;
    QString t;
    QString commentMark;
    if (cppExts.contains(ext)) {
        commentMark = "//";
    } else if (fortranExts.contains(ext)) {
        commentMark = "!";
    } else if (asmExts.contains(ext)) {
        commentMark = ";";
    } else {
        return;
    }
    for (int i = startBlock; i <= endBlock; i++) {
        block = doc->findBlockByNumber(i);
        pos = block.position();
        cursor.setPosition(pos);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,
                commentMark.length());
        t = cursor.selectedText();
        if (t == commentMark) cursor.deleteChar();
    }
    cursor.setPosition(start);
    cursor.setPosition(end, QTextCursor::KeepAnchor);

}

void SourceWindow::indent()
{
    int start;
    int end;
    tab_width = ebe["edit/tab_width"].toInt();
    QTextCursor cursor = textEdit->textCursor();
    QTextDocument *doc = textEdit->document();
    if (cursor.hasSelection()) {
        start = cursor.selectionStart();
        end = cursor.selectionEnd();
    } else {
        start = end = cursor.position();
    }
    QTextBlock block;
    block = doc->findBlock(start);
    int startBlock = block.blockNumber();
    block = doc->findBlock(end);
    int endBlock = block.blockNumber();
    int n = file.source.lastIndexOf('.');
    if (n < 0) {
        qDebug() << "File does not have an extension.";
        return;
    }
    QString ext = file.source.mid(n + 1);
    int pos;
    QString prefix;
    for (int i = 0; i < tab_width; i++)
        prefix += " ";
    for (int i = startBlock; i <= endBlock; i++) {
        block = doc->findBlockByNumber(i);
        pos = block.position();
        cursor.setPosition(pos);
        cursor.insertText(prefix);
    }

}

void SourceWindow::unIndent()
{
    tab_width = ebe["edit/tab_width"].toInt();
    QTextCursor cursor = textEdit->textCursor();
    QTextDocument *doc = textEdit->document();
    int start;
    int end;
    if (cursor.hasSelection()) {
        start = cursor.selectionStart();
        end = cursor.selectionEnd();
    } else {
        start = end = cursor.position();
    }
    QTextBlock block;
    block = doc->findBlock(start);
    int startBlock = block.blockNumber();
    block = doc->findBlock(end);
    int endBlock = block.blockNumber();
    int n = file.source.lastIndexOf('.');
    if (n < 0) {
        qDebug() << "File does not have an extension.";
        return;
    }
    QString ext = file.source.mid(n + 1);
    int pos;
    QString t;
    QString prefix;
    for (int i = 0; i < tab_width; i++)
        prefix += " ";
    for (int i = startBlock; i <= endBlock; i++) {
        block = doc->findBlockByNumber(i);
        pos = block.position();
        cursor.setPosition(pos);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, tab_width);
        t = cursor.selectedText();
        if (t == prefix) cursor.deleteChar();
    }
    cursor.setPosition(start);
    cursor.setPosition(end, QTextCursor::KeepAnchor);

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
    if (blockNumber + textHeight >= doc->lineCount()) {
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
    if (blockNumber - textHeight < 0) {
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
    if (blockNumber > textHeight / 2) {
        top = blockNumber - textHeight / 2;
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
    int blockNumber = doc->lineCount() - 1;
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
    int blockNumber = scrollBar->value() + textHeight - 2;
    QTextBlock block;
    block = doc->findBlockByNumber(blockNumber);
    int pos = block.position();
    cursor.setPosition(pos);
    textEdit->setTextCursor(cursor);
    setLineNumbers(textDoc->lineCount());
}

void SourceWindow::gotoLine()
{
    //qDebug() << "gotoLine";
    QTextDocument *doc = textEdit->document();
    LineNumberDialog *dialog = new LineNumberDialog;
    QTextCursor cursor = textEdit->textCursor();
    int pos = cursor.position();
    QTextBlock block;
    block = doc->findBlock(pos);
    dialog->line = block.blockNumber() + 1;
    dialog->setMax(doc->lineCount());
    if (dialog->exec()) {
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
    cmd.replace("$source", file.source);
    cmd.replace("$tab_width", ebe["edit/tab_width"].toString());
    indent.start(cmd);
    indent.waitForFinished();
    open(file.source);
}

void LineNumberEdit::mouseReleaseEvent(QMouseEvent *e)
{
    QString s;
    //SourceWindow *p = (SourceWindow *)parent();
    //int row = (e->pos().y()-2)/p->fontHeight + p->topNumber;
    int block = cursorForPosition(e->pos()).blockNumber();
    //qDebug() << "mre" << row;
    //qDebug() << "block" << block;
    if (breakpoints->contains(block + 1)) {
        breakpoints->remove(block + 1);
        cursorForPosition(e->pos()).setBlockFormat(normalFormat);
        emit deleteBreakpoint(myParent->file.source, s.setNum(block + 1));
    } else {
        breakpoints->insert(block + 1);
        cursorForPosition(e->pos()).setBlockFormat(breakFormat);
        emit sendBreakpoint(myParent->file.source, s.setNum(block + 1));
    }
    //foreach ( int line, *breakpoints ) {
    //qDebug() << "bp at" << line;
    //}
}

void LineNumberEdit::contextMenuEvent(QContextMenuEvent *event)
{
    eventPosition = event->pos();
    QMenu menu("Breakpoint menu");
    menu.addAction(tr("Set breakpoint"), this, SLOT(setBreakpoint()));
    menu.addAction(tr("Drop breakpoint"), this, SLOT(dropBreakpoint()));
    menu.addAction(tr("Drop all breakpoints"), this, SLOT(dropAllBreakpoints()));
    menu.addAction(tr("ignore"), this, SLOT(ignore()));
    menu.exec(QCursor::pos());
}

void LineNumberEdit::ignore()
{
}

void LineNumberEdit::dropAllBreakpoints()
{
    QString s;
    //SourceWindow *p = (SourceWindow *)parent();
    //qDebug() << "top" << p->topNumber;
    foreach ( int line, *breakpoints )
    {
        //qDebug() << "bp at" << line;
        breakpoints->remove(line);
        emit deleteBreakpoint(myParent->file.source,s.setNum(line));
        //eventPosition.setX(0);
        //eventPosition.setY((line-p->topNumber)*p->fontHeight+p->fontHeight/2+1);
        //qDebug() << "Pos" << eventPosition;
        QTextCursor(document()->findBlockByNumber(line-1)).setBlockFormat(normalFormat);
    }
}

void LineNumberEdit::setBreakpoint()
{
    QString s;
    //SourceWindow *p = (SourceWindow *)parent();
    //int row = (eventPosition.y()-2)/p->fontHeight + p->topNumber;
    int block = cursorForPosition(eventPosition).blockNumber();
    //qDebug() << "set" << row;
    //qDebug() << "block" << block;
    breakpoints->insert(block + 1);
    emit sendBreakpoint(myParent->file.source, s.setNum(block + 1));
    cursorForPosition(eventPosition).setBlockFormat(breakFormat);
}

void LineNumberEdit::dropBreakpoint()
{
    QString s;
    //qDebug() << "drop" << eventPosition;
    //SourceWindow *p = (SourceWindow *)parent();
    //int row = (eventPosition.y()-2)/p->fontHeight + p->topNumber;
    int block = cursorForPosition(eventPosition).blockNumber();
    //qDebug() << "set" << row;
    //qDebug() << "block" << block;
    breakpoints->remove(block + 1);
    emit deleteBreakpoint(myParent->file.source, s.setNum(block + 1));
    cursorForPosition(eventPosition).setBlockFormat(normalFormat);
}

void LineNumberEdit::wheelEvent(QWheelEvent * /* e */)
{
}

    LineNumberDialog::LineNumberDialog()
: QDialog()
{
    setObjectName("Go to line");
    setWindowTitle("Go to line");
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    move(QCursor::pos());
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10, 10, 10, 10);

    QHBoxLayout *lineLayout = new QHBoxLayout;
    lineLayout->addWidget(new QLabel(tr("Line number")));
    lineSpin = new QSpinBox;
    lineSpin->setMinimum(1);
    lineLayout->addWidget(lineSpin);
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
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);

    move(QCursor::pos());
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10, 10, 10, 10);

    QHBoxLayout *findLayout = new QHBoxLayout;
    findLayout->addWidget(new QLabel(tr("Find")));
    findEdit = new QLineEdit;
    findLayout->addWidget(findEdit);
    layout->addLayout(findLayout);

    QHBoxLayout *replaceLayout = new QHBoxLayout;
    replaceLayout->addWidget(new QLabel(tr("Replace")));
    replaceEdit = new QLineEdit;
    replaceLayout->addWidget(replaceEdit);
    layout->addLayout(replaceLayout);

    findButton = new QPushButton("Find");
    findButton->setShortcut(QKeySequence(tr("Ctrl+F")));
    replaceButton = new QPushButton("Replace");
    replaceButton->setShortcut(QKeySequence(tr("Ctrl+R")));
    cancelButton = new QPushButton("Cancel");
    cancelButton->setShortcut(QKeySequence(tr("Ctrl+C")));
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
    if (!textEdit->find(findEdit->text())) {
        textEdit->moveCursor(QTextCursor::Start);
        textEdit->find(findEdit->text());
    }
}

void FindReplaceDialog::replace()
{
    textEdit->insertPlainText(replaceEdit->text());
}
