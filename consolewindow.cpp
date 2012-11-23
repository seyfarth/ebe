#include "consolewindow.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextCursor>
#include <QApplication>
#include <QLabel>
#include <QFile>


ConsoleWindow::ConsoleWindow(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Plain );
    setLineWidth(1);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(3,3,3,3);

    QHBoxLayout *commandLayout = new QHBoxLayout;
    QLabel *label = new QLabel("gdb command");
    commandLayout->addWidget(label);
    commandLine = new QLineEdit(this);
    commandLayout->addWidget(commandLine);

    textEdit = new QPlainTextEdit(this);
    layout->addWidget(textEdit);
    layout->addLayout(commandLayout);
    setLayout(layout);
}
