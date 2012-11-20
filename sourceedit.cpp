#include "sourceedit.h"
#include <QTextEdit>
#include <QKeyEvent>


SourceEdit::SourceEdit(QWidget *parent) : QTextEdit(parent)
{
}

void SourceEdit::keyPressEvent ( QKeyEvent *event )
{
    if ( event->matches(QKeySequence::ZoomIn) ) event->ignore();
}
