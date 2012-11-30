#include "terminalwindow.h"
#include <QKeyEvent>


TerminalWindow::TerminalWindow(QWidget *parent)
: QPlainTextEdit(parent)
{
    setObjectName("Terminal");

    //setFrameStyle( QFrame::Box | QFrame::Raised );
    //setLineWidth(4);
    //setContentsMargins(10,10,10,10);
}

void TerminalWindow::keyPressEvent ( QKeyEvent *event )
{
    //if ( event->matches(QKeySequence::ZoomIn) ) event->ignore();
    QPlainTextEdit::keyPressEvent(event);
}
