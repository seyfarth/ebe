#include "sourcewindow.h"
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QTextCursor>

SourceWindow::SourceWindow(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Plain );
    setLineWidth(2);

    createLineNumberEdit();
    createTextEdit();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(3,3,3,3);
    layout->addWidget(lineNumberEdit,0);
    layout->addWidget(textEdit,1);

    setLayout(layout);
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

