#ifndef FLOATWINDOW_H
#define FLOATWINDOW_H

#include <QScrollArea>
#include <QString>
#include "listwidget.h"
#include <QListWidgetItem>

class FloatWindow : public QScrollArea
{
    Q_OBJECT

public:
    FloatWindow(QWidget *parent=0);

private slots:

private:
    void setRegister ( int n, QString value );
    QListWidgetItem *regs[16];
    ListWidget *names1;
    ListWidget *values1;
    ListWidget *names2;
    ListWidget *values2;
};

#endif
