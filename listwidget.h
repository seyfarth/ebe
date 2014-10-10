#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>
#include <QWheelEvent>

class ListWidget: public QListWidget
{
    Q_OBJECT

public:
    ListWidget(QWidget *parent = 0);

private slots:

private:
    void wheelEvent ( QWheelEvent *event );
};

#endif
