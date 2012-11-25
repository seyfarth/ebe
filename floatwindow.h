#ifndef FLOATWINDOW_H
#define FLOATWINDOW_H

#include <QScrollArea>
#include <QString>
#include <QListWidget>
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
    QListWidget *names1;
    QListWidget *values1;
    QListWidget *names2;
    QListWidget *values2;
};

#endif
