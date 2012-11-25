#ifndef FLOATWINDOW_H
#define FLOATWINDOW_H

#include <QFrame>
#include <QString>
#include <QListWidget>


class FloatWindow : public QFrame
{
    Q_OBJECT

public:
    FloatWindow(QWidget *parent=0);

private slots:

private:
    void addRegister ( int n, QString value );
    QListWidget *names1;
    QListWidget *values1;
    QListWidget *names2;
    QListWidget *values2;
};

#endif
