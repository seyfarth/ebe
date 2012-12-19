#ifndef FLOATWINDOW_H
#define FLOATWINDOW_H

#include <QtGui>

class FpRegister
{
public:
    FpRegister();
    QString value();
    void setValue(unsigned long *x);
    void setFormat(QString f);
    
private:
    union {
        double f8[4];
        float  f4[8];
        unsigned long u8[4];
        long   i8[4];
        unsigned int  u4[8];
        int    i4[8];
        unsigned short u2[16];
        signed short   i2[16];
        unsigned char  u1[32];
        signed char    i1[32];
    };
    QString format;
};

class FloatWindow : public QFrame
{
    Q_OBJECT

public:
    FloatWindow(QWidget *parent=0);
    void setFontHeightAndWidth ( int height, int width );
    void setRegister ( int n, QString value );

public slots:
    void receiveFpRegs(QStringList);
    void formatRegister(QAction *action);
    void formatAllRegisters(QAction *action);

private:
    QTableWidgetItem *regs[16];
    QTableWidget *table;
    QSize sizeHint() const;
    FpRegister regValues[16];
    int fontHeight;
    int fontWidth;
    int currentColumn;
    int currentRow;
    void contextMenuEvent ( QContextMenuEvent *event );
};

#endif
