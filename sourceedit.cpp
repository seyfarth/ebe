#include "sourceedit.h"
#include <QTextEdit>
#include <QKeyEvent>


SourceEdit::SourceEdit(QWidget *parent) : QTextEdit(parent)
{
}

void SourceEdit::keyPressEvent ( QKeyEvent *event )
{
    printf("Got key\n");
    event->ignore();
}
