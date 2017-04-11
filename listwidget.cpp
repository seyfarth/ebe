#include "listwidget.h"
#include <cstdio>

ListWidget::ListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setStyleSheet("QListWidget{border: 0px}");
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void ListWidget::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}
