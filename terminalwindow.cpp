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

    ptyName = ptsname(pty);
    //qDebug() << "pty" << ptyName;
    ptyReader = new PtyReader(pty,ptyName);
    connect(lineEdit,SIGNAL(returnPressed()),this,SLOT(lineEditReady()));
    connect(ptyReader,SIGNAL(dataReady(QString)),
            this,SLOT(dataReady(QString)) );
    ptySlave = open(ptsname(pty),O_RDWR);
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
    if ( write(pty,a.data(),a.length()) < 1 ) {
        qDebug() << "error writing to pty on lineEditReady";
    }
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
