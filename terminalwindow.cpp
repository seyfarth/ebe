#include "terminalwindow.h"
#include "ptyreader.h"
#include "settings.h"
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
    if ( !CreatePipe(&fromChild,&childStdout,&attr,4096) ) {
       qDebug() << "Could not create pipe from child";
       return;
    }
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

    lineEdit = new QLineEdit;
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
    ptyReader->start();
}

void TerminalWindow::dataReady(QString data)
{
    int n = data.length();
    if (data.at(n-1) == '\n') {
        data.chop(1);
        if (data.at(n-2) == '\r') data.chop(1);
        //qDebug() << "chop" << data;
    }
    edit->appendPlainText(data);
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
    if ( ! WriteFile(toChild,a.data(),n,&res,NULL) ) {
        qDebug() << "error writing to child on lineEditReady";
    }
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
