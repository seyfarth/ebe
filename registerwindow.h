#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

/*
    RegisterWindow class
    For display of general purpose registers, no public functions
    for set and get registers just yet
*/

#include <QMap>
#include <QFrame>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QString>
#include <QSize>

class RegisterWindow : public QFrame
{
    Q_OBJECT

public:
    RegisterWindow(QWidget *parent = 0);
    void setFontHeightAndWidth ( int height, int width );
    void setRegister ( QString name, QString value );

private:
    QMap<QString,QTableWidgetItem *> registerMap;
    QTableWidget *table;
    QSize sizeHint();

private slots:

};

#endif

