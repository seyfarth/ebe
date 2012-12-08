#include "terminalwindow.h"
#include "ptyreader.h"
#include "settings.h"
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QDebug>
#define _XOPEN_SOURCE
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


TerminalWindow::TerminalWindow(QWidget *parent)
: QFrame(parent)
{
    setObjectName("Terminal");

    setFrameStyle( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    pty = posix_openpt(O_RDWR);
    grantpt(pty);
    unlockpt(pty);
    
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(10,10,10,10);

    edit = new TerminalEdit(pty,this);

    layout->addWidget(edit);

    setLayout(layout);

    ptyName = ptsname(pty);
    qDebug() << "pty" << ptyName;
    ptyReader = new PtyReader(pty,ptyName,this);
    connect(ptyReader,SIGNAL(dataReady(char*,int)),
            this,SLOT(dataReady(char*,int)) );
    ptySlave = open(ptsname(pty),O_RDWR);
    ptyReader->start();
}

void TerminalWindow::dataReady(char *data, int n)
{
    data[n] = 0;
    edit->appendPlainText(data);
}

TerminalEdit::TerminalEdit(int fd,QWidget *parent)
: QPlainTextEdit(parent)
{
    pty = fd;
}

void TerminalEdit::keyPressEvent ( QKeyEvent *event )
{
    unsigned char c = event->key();
    write(pty,&c,1);
    QPlainTextEdit::keyPressEvent(event);
}
