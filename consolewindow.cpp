#include "consolewindow.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextCursor>
#include <QApplication>
#include <QKeyEvent>
#include <QFile>


ConsoleWindow::ConsoleWindow(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Plain );
    setLineWidth(1);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(3,3,3,3);

    textEdit = new QPlainTextEdit(this);
    commandLine = new QLineEdit(this);
    layout->addWidget(textEdit);
    layout->addWidget(commandLine);
    setLayout(layout);
}
