#include "projectwindow.h"
#include <QKeyEvent>


ProjectWindow::ProjectWindow(QWidget *parent)
: QPlainTextEdit(parent)
{
}

void ProjectWindow::keyPressEvent ( QKeyEvent *event )
{
    //if ( event->matches(QKeySequence::ZoomIn) ) event->ignore();
    QPlainTextEdit::keyPressEvent(event);
}
