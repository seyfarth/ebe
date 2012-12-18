#include "floatwindow.h"
#include "settings.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QDebug>
#include <cstdio>

static QString names[8][2] = {
    { "xmm0", "xmm8" },
    { "xmm1", "xmm9" },
    { "xmm2", "xmm10" },
    { "xmm3", "xmm11" },
    { "xmm4", "xmm12" },
    { "xmm5", "xmm13" },
    { "xmm6", "xmm14" },
    { "xmm7", "xmm15" }
};

FloatWindow::FloatWindow(QWidget *parent)
: QFrame(parent)
{
    setObjectName("Float");
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(10,10,10,10);

    table = new QTableWidget(this);
    table->setRowCount(8);
    table->setColumnCount(4);
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();

    QTableWidgetItem *name;
    QTableWidgetItem *value;
    for ( int r = 0; r < 8; r++ ) {
        for ( int c = 0; c < 2; c++ ) {
            name = new QTableWidgetItem(names[r][c]+QString(" "));
            name->setTextAlignment(Qt::AlignRight);
            value = new QTableWidgetItem("0.0");
            regs[c*8+r] = value;
            table->setItem(r,c*2,name);
            table->setItem(r,c*2+1,value);
        }
    }
    layout->addWidget(table);

    table->setShowGrid(false);
    table->resizeRowsToContents();
    table->resizeColumnsToContents();

    setLayout ( layout );
}

QSize FloatWindow::sizeHint() const
{
    return QSize(300,100);
}

void FloatWindow::setFontHeightAndWidth ( int height, int width )
{
    int max, length;
    fontHeight = height;
    fontWidth  = width;
    for ( int r = 0; r < 8; r++ ) {
        table->setRowHeight(r,height+3);
    }
    for ( int c = 0; c < 4; c++ ) {
        max = 1;
        for ( int r = 0; r < 8; r++ ) {
            length = table->item(r,c)->text().length();
            if ( length > max ) max = length;
        }
        table->setColumnWidth(c,(max+1)*width+3);
    }

}

void FloatWindow::setRegister ( int n, QString value )
{
    if ( n >= 0 && n < 16 ) regs[n]->setText(value);
}

void FloatWindow::receiveFpRegs ( QStringList data )
{
    QStringList parts;
    bool ok;
    unsigned long x[4]={0,0,0,0};
    qDebug() << "receive" << data;
    for (int i = 0; i < 16; i++ ) {
        parts = data[i].split(QRegExp("\\s+"));
        qDebug() << parts;
        for ( int j = 0; j < parts.length(); j++ ) {
            x[j] = parts[j].mid(2).toLong(&ok,16);
        }
        qDebug() << "val" << i << x[0] << x[1] << x[2] << x[3];
        regValues[i].setValue(x);
        setRegister(i,regValues[i].value());
    }
    setFontHeightAndWidth(fontHeight,fontWidth);
}

FpRegister::FpRegister()
{
    format = "double";
    for ( int i = 0; i < 4; i++ ) i8[i] = 0;
}

void FpRegister::setValue(unsigned long *x)
{
    for ( int i = 0; i < 4; i++ ) u8[i] = x[i];
}

QString FpRegister::value()
{
    QString s;
    s.sprintf("%12g",f8[0]);
    return s;
}

void FpRegister::setFormat(QString f)
{
    format = f;
}
