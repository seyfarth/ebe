#ifndef FLOATWINDOW_H
#define FLOATWINDOW_H

#include <QtGui>
#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>

class FpRegister
{
public:
    FpRegister();
    QString value();
#ifdef Q_WS_WIN
    void setValue(unsigned long long *x);
#else
    void setValue(unsigned long *x);
#endif
    void setFormat(QString f);

private:
    union
    {
        double f8[4];
        float f4[8];
#ifdef Q_WS_WIN
        unsigned long long u8[4];
#else
        unsigned long u8[4];
#endif
#ifdef Q_WS_WIN
        long long i8[4];
#else
        long i8[4];
#endif
        unsigned int u4[8];
        int i4[8];
        unsigned short u2[16];
        signed short i2[16];
        unsigned char u1[32];
        signed char i1[32];
    };
    QString format;
};

class FloatWindow: public QFrame
{
    Q_OBJECT

public:
    FloatWindow(QWidget *parent = 0);
    void resetNames();
    void setFontHeightAndWidth(int height, int width);
    void setRegister(int n, QString value);
    int count;
    QTableWidget *table;

public slots:
    void receiveFpRegs(QStringList);
    void formatRegister(QAction *action);
    void formatAllRegisters(QAction *action);

private:
    QTableWidgetItem *regs[16];
    QSize sizeHint() const;
    FpRegister regValues[16];
    int fontHeight;
    int fontWidth;
    int currentColumn;
    int currentRow;
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif
