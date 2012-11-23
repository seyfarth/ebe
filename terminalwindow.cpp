#include "terminalwindow.h"
#include <QKeyEvent>


TerminalWindow::TerminalWindow(QWidget *parent)
: QPlainTextEdit(parent)
{
}

void TerminalWindow::keyPressEvent ( QKeyEvent *event )
{
    //if ( event->matches(QKeySequence::ZoomIn) ) event->ignore();
    QPlainTextEdit::keyPressEvent(event);
}
