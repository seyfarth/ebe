#include "terminalwindow.h"
#include <QKeyEvent>
#include <QHBoxLayout>


TerminalWindow::TerminalWindow(QWidget *parent)
: QFrame(parent)
{
    setObjectName("Terminal");

    setFrameStyle( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);
    setMidLineWidth(1);
    setContentsMargins(10,10,10,10);

    QHBoxLayout *layout = new QHBoxLayout;

    edit = new TerminalEdit(this);

    layout->addWidget(edit);

    setLayout(layout);

}

TerminalEdit::TerminalEdit(QWidget *parent)
: QPlainTextEdit(parent)
{
}

void TerminalEdit::keyPressEvent ( QKeyEvent *event )
{
    //if ( event->matches(QKeySequence::ZoomIn) ) event->ignore();
    QPlainTextEdit::keyPressEvent(event);
}
