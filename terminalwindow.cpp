#include "terminalwindow.h"
#include "ptyreader.h"
#include "settings.h"
#include "gdb.h"
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#define _XOPEN_SOURCE
#include <stdlib.h>
#include <fcntl.h>
#ifndef Q_WS_WIN
#include <unistd.h>
#endif

extern GDB *gdb;

TerminalWindow::TerminalWindow(QWidget *parent)
: QFrame(parent)
{
    setObjectName("Terminal");

    setFrameStyle( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

#ifdef Q_WS_WIN
    SECURITY_ATTRIBUTES attr;
    attr.nLength = sizeof(SECURITY_ATTRIBUTES);
    attr.lpSecurityDescriptor = 0;
    attr.bInheritHandle = TRUE;
    if ( !CreatePipe(&childStdin,&toChild,&attr,4096) ) {
       qDebug() << "Could not create pipe to child";
       return;
    }
    SetHandleInformation(toChild,HANDLE_FLAG_INHERIT,4096);
    if ( !CreatePipe(&fromChild,&childStdout,&attr,1) ) {
       qDebug() << "Could not create pipe from child";
       return;
    }
    SetHandleInformation(fromChild,HANDLE_FLAG_INHERIT,0);
#else
    pty = posix_openpt(O_RDWR);
    grantpt(pty);
    unlockpt(pty);
#endif
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(10,10,10,10);

    edit = new TerminalEdit(pty,this);
    edit->setReadOnly(true);

    layout->addWidget(edit);

    lineEdit = new InputEdit;
    QLabel *label = new QLabel(tr("Input"));
    QHBoxLayout *lineLayout = new QHBoxLayout;
    lineLayout->addWidget(label);
    lineLayout->addWidget(lineEdit);
    lineEdit->setToolTip(
        tr("Terminal input will only work from this input box."));

    layout->addLayout(lineLayout);
    setLayout(layout);

#ifdef Q_WS_WIN
    ptyReader = new PtyReader(fromChild);
#else
    ptyName = ptsname(pty);
    //qDebug() << "pty" << ptyName;
    ptyReader = new PtyReader(pty);
    ptySlave = open(ptsname(pty),O_RDWR);
#endif
    connect(lineEdit,SIGNAL(returnPressed()),this,SLOT(lineEditReady()));
    connect(ptyReader,SIGNAL(dataReady(QString)),
            this,SLOT(dataReady(QString)) );
    connect(lineEdit,SIGNAL(sendEOF()),gdb,SLOT(setEOF()));
    ptyReader->start();
}

void TerminalWindow::dataReady(QString data)
{
    edit->textCursor().insertText(data);
    edit->ensureCursorVisible();
}

InputEdit::InputEdit()
: QLineEdit()
{
    flashCount = 0;
    timer = new QTimer(this);
    timer->stop();
    connect(timer,SIGNAL(timeout()),this,SLOT(updateFlash()));
    connect(gdb,SIGNAL(endFlash()),this,SLOT(endFlash()));
}

void InputEdit::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    bool control = (e->modifiers() & Qt::ControlModifier) != 0;
    if ( control &&
        (key == Qt::Key_D || key == Qt::Key_C || key == Qt::Key_Z) ) {
        startFlash();
        emit sendEOF();
    } else {
        QLineEdit::keyPressEvent(e);
    }
}

void InputEdit::startFlash()
{
    timer->start(25);
}

void InputEdit::updateFlash()
{
    QPalette p=palette();
    p.setColor(QPalette::Base,
          QColor(80*sin(flashCount*0.023)+150,
                 80*sin(flashCount*0.016+0.5)+150,
                 60*sin(flashCount*0.021+1.0)+80));
    setPalette(p);
    flashCount++;
}

void InputEdit::endFlash()
{
    timer->stop();
    QPalette p=palette();
    p.setColor(QPalette::Base, QColor("white"));
    setPalette(p);
}

void TerminalWindow::lineEditReady()
{
    QString s;
    QByteArray a;
    s = lineEdit->text();
    a = s.toAscii();
    a.append('\n');
#ifdef Q_WS_WIN
    DWORD n=a.length();
    DWORD res;
    if ( s == "EOF" ) {
        a[0] = 3;
        a[1] = '\n';
        WriteFile(toChild,a.data(),2,&res,NULL);
    } else if ( ! WriteFile(toChild,a.data(),n,&res,NULL) ) {
        qDebug() << "error writing to child on lineEditReady";
    }
    dataReady(s+"\n");
#else
    if ( write(pty,a.data(),a.length()) < 1 ) {
        qDebug() << "error writing to pty on lineEditReady";
    }
#endif
    lineEdit->clear();
}

TerminalEdit::TerminalEdit(int fd,QWidget *parent)
: QPlainTextEdit(parent)
{
    pty = fd;
}

//void TerminalEdit::keyPressEvent ( QKeyEvent *event )
//{
    //unsigned char c = event->key();
    //qDebug() << "key" << event->key();
    //write(pty,&c,1);
    //QPlainTextEdit::keyPressEvent(event);
//}
