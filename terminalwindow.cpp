#include "terminalwindow.h"
#include <QKeyEvent>


TerminalWindow::TerminalWindow(QWidget *parent)
: QPlainTextEdit(parent)
{
    setObjectName("Terminal");
}

void TerminalWindow::keyPressEvent ( QKeyEvent *event )
{
    //if ( event->matches(QKeySequence::ZoomIn) ) event->ignore();
    QPlainTextEdit::keyPressEvent(event);
}
