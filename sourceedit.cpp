#include "sourceedit.h"
#include <QPlainTextEdit>
#include <QTextOption>
#include <QScrollBar>
#include <QKeyEvent>
#include <QEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QDebug>
#include <QtGui>


SourceEdit::SourceEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    setWordWrapMode(QTextOption::NoWrap);
    //QTimer *timer = new QTimer(this);
    //connect(timer,SIGNAL(timeout()),this,SLOT(printScroll()));
    //timer->start(1000);
    top = -1;
    scrollBar = verticalScrollBar();
}

//void SourceEdit::scrollContentsBy ( int dx, int dy )
//{
    //QPlainTextEdit::scrollContentsBy(dx,dy);
    //emit newHeight(heightInPixels);
//}
void SourceEdit::wheelEvent ( QWheelEvent *event )
{
    QPlainTextEdit::wheelEvent(event);
    int x = scrollBar->value();
    if ( x != top ) {
        emit newHeight(heightInPixels);
        top = x;
    }
}

bool SourceEdit::event ( QEvent *event )
{
    bool ret = QPlainTextEdit::event(event);
    int x = scrollBar->value();
    if ( x != top ) {
        emit newHeight(heightInPixels);
        top = x;
    }
    return ret;
}

void SourceEdit::keyPressEvent ( QKeyEvent *event )
{
    //if ( event->matches(QKeySequence::ZoomIn) ) event->ignore();
    QPlainTextEdit::keyPressEvent(event);
}

void SourceEdit::printScroll()
{
    QScrollBar *sb = verticalScrollBar();
    qDebug() << "sb" << sb->minimum() << sb->value() << sb->maximum() << endl;
    qDebug() << viewport()->size();
    qDebug() << "blocks" << blockCount();
}

void SourceEdit::resizeEvent(QResizeEvent *e)
{
    heightInPixels = e->size().height();
    //qDebug() << "Size " << e->size();
    //qDebug() << "ht " << heightInPixels;
    emit newHeight(heightInPixels);
}
