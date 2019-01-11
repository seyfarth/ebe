#include "consolewindow.h"
#include "debugger.h"
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
#include <QDebug>

extern Debugger *dbg;

ConsoleWindow::ConsoleWindow(QWidget *parent)
    : QFrame(parent)
{
    setObjectName("Console");
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(4);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(10, 10, 10, 10);

    QHBoxLayout *commandLayout = new QHBoxLayout;
    QLabel *label = new QLabel(tr("debugger command"));
    label->setStyleSheet("font-family: " + ebe["variable_font"].toString());
    commandLayout->addWidget(label);
    commandLine = new QLineEdit(this);
    commandLayout->addWidget(commandLine);

    textEdit = new QPlainTextEdit(this);
    layout->addWidget(textEdit);
    layout->addLayout(commandLayout);
    setLayout(layout);

    scrollBar = textEdit->verticalScrollBar();

    connect(dbg, SIGNAL(log(QString)), this, SLOT(log(QString)),
            Qt::QueuedConnection);
    connect(commandLine, SIGNAL(returnPressed()), this, SLOT(sendCommand()));
    connect(this, SIGNAL(doCommand(QString)), dbg, SLOT(doCommand(QString)),
            Qt::BlockingQueuedConnection);
}

void ConsoleWindow::log(QString s)
{
    //qDebug() << "logging" << s;
    textEdit->textCursor().insertText(s + '\n');
    scrollBar->setValue(textEdit->document()->lineCount() - 1);
}

void ConsoleWindow::sendCommand()
{
    emit doCommand(commandLine->text());
}
