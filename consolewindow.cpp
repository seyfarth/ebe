#include "consolewindow.h"
#include "gdb.h"
#include "settings.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollBar>
#include <QTextCursor>
#include <QApplication>
#include <QLabel>
#include <QFile>

extern GDB *gdb;

ConsoleWindow::ConsoleWindow(QWidget *parent) : QFrame(parent)
{
    setObjectName("Console");
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10,10,10,10);

    QHBoxLayout *commandLayout = new QHBoxLayout;
    QLabel *label = new QLabel("gdb command");
    label->setStyleSheet ( "font-family: " +
                            ebe["variable_font"].toString() );
    commandLayout->addWidget(label);
    commandLine = new QLineEdit(this);
    commandLayout->addWidget(commandLine);

    textEdit = new QPlainTextEdit(this);
    layout->addWidget(textEdit);
    layout->addLayout(commandLayout);
    setLayout(layout);

    scrollBar = textEdit->verticalScrollBar();

    connect(gdb,SIGNAL(log(QString)),this,SLOT(log(QString)));
    connect(commandLine,SIGNAL(returnPressed()),this,SLOT(sendCommand()));
    connect(this,SIGNAL(doCommand(QString)),gdb,SLOT(doCommand(QString)));
}

void ConsoleWindow::log ( QString s )
{
    textEdit->textCursor().insertText(s+'\n');
    scrollBar->setValue(textEdit->document()->lineCount()-1);
}

void ConsoleWindow::sendCommand()
{
    emit doCommand(commandLine->text());
}
