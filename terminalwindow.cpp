#include "terminalwindow.h"
#include "settings.h"
#include <QKeyEvent>
#include <QHBoxLayout>


TerminalWindow::TerminalWindow(QWidget *parent)
: QFrame(parent)
{
    setObjectName("Terminal");

    setFrameStyle( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(10,10,10,10);

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
