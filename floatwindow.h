#ifndef FLOATWINDOW_H
#define FLOATWINDOW_H

#include <QSize>
#include <QFrame>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>

class FloatWindow : public QFrame
{
    Q_OBJECT

public:
    FloatWindow(QWidget *parent=0);
    void setFontHeightAndWidth ( int height, int width );
    void setRegister ( int n, QString value );

private slots:

private:
    QTableWidgetItem *regs[16];
    QTableWidget *table;
    QSize sizeHint();
};

#endif
