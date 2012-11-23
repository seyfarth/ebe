#include "sourceedit.h"
#include <QPlainTextEdit>
#include <QKeyEvent>


SourceEdit::SourceEdit(QWidget *parent) : QPlainTextEdit(parent)
{
}

void SourceEdit::keyPressEvent ( QKeyEvent *event )
{
    //if ( event->matches(QKeySequence::ZoomIn) ) event->ignore();
    QPlainTextEdit::keyPressEvent(event);
}
