#include "sourcewindow.h"
#include "commandline.h"
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextCursor>

SourceWindow::SourceWindow(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Plain );
    setLineWidth(2);

    createLineNumberEdit();
    createTextEdit();

    QVBoxLayout *sourceLayout = new QVBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    quitButton     = new QPushButton ( "Quit" );
    runButton      = new QPushButton ( "Run" );
    nextButton     = new QPushButton ( "Next" );
    stepButton     = new QPushButton ( "Step" );
    continueButton = new QPushButton ( "Continue" );
    stopButton     = new QPushButton ( "Stop" );
    buttonLayout->addWidget ( quitButton );
    buttonLayout->addWidget ( runButton );
    buttonLayout->addWidget ( nextButton );
    buttonLayout->addWidget ( stepButton );
    buttonLayout->addWidget ( continueButton );
    buttonLayout->addWidget ( stopButton );

    connect ( quitButton, SIGNAL(clicked()), parent, SLOT(quit()) );

    commandLine = new CommandLine();

    QHBoxLayout *editorLayout = new QHBoxLayout;
    editorLayout->setSpacing(2);
    editorLayout->setContentsMargins(3,3,3,3);

    editorLayout->addWidget(lineNumberEdit,0);
    editorLayout->addWidget(textEdit,1);

    sourceLayout->addLayout(buttonLayout);
    sourceLayout->addWidget(commandLine);
    sourceLayout->addLayout(editorLayout);
    setLayout(sourceLayout);
}

void SourceWindow::createLineNumberEdit()
{
    lineNumberEdit = new QTextEdit(this);
    lineNumberEdit->setFixedWidth(60);
    lineNumberEdit->append(QString("1234"));
}

void SourceWindow::createTextEdit()
{
    textEdit = new QTextEdit(this);
    textEdit->append("Text");
}

void SourceWindow::createButtons()
{
}

void SourceWindow::createCommandLineEdit()
{
}

