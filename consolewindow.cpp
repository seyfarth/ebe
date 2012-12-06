#include "consolewindow.h"
#include "settings.h"
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
}
