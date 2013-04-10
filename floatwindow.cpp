#include "floatwindow.h"
#include "settings.h"
#include <cstdio>


FloatWindow::FloatWindow(QWidget *parent)
: QFrame(parent)
{
    setObjectName("Float");
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    count = wordSize == 64 ? 16 : 8;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(10,10,10,10);

    table = new QTableWidget(this);
    table->setRowCount(count/2);
    table->setColumnCount(4);
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    table->setToolTip(tr("Right click to change register formats."));

    QTableWidgetItem *name;
    QTableWidgetItem *value;
    for ( int r = 0; r < count/2; r++ ) {
        for ( int c = 0; c < 2; c++ ) {
            name = new QTableWidgetItem(QString("xmm%1 ").arg(c*(count/2)+r));
            name->setTextAlignment(Qt::AlignRight);
            value = new QTableWidgetItem("0.0");
            regs[c*(count/2)+r] = value;
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
    return QSize(200,10);
}

void FloatWindow::setFontHeightAndWidth ( int height, int width )
{
    int max, length;
    fontHeight = height;
    fontWidth  = width;
    for ( int r = 0; r < count/2; r++ ) {
        table->setRowHeight(r,height+3);
    }
    for ( int c = 0; c < 4; c++ ) {
        max = 1;
        for ( int r = 0; r < count/2; r++ ) {
            length = table->item(r,c)->text().length();
            if ( length > max ) max = length;
        }
        table->setColumnWidth(c,(max+1)*width+3);
    }

}

void FloatWindow::setRegister ( int n, QString value )
{
    if ( n >= 0 && n < count ) regs[n]->setText(value);
}

void FloatWindow::receiveFpRegs ( QStringList data )
{
    QStringList parts;
    bool ok;
#ifdef Q_WS_WIN
    unsigned long long x[4]={0,0,0,0};
#else
    unsigned long x[4]={0,0,0,0};
#endif
    //qDebug() << "fp receive" << data;
    if ( data.length() < count ) {
        qDebug() << "fpreg error";
        return;
    }
    for (int i = 0; i < count; i++ ) {
        parts = data[i].split(QRegExp("\\s+"));
        //qDebug() << parts;
        for ( int j = 0; j < parts.length(); j++ ) {
#ifdef Q_WS_WIN
            x[j] = parts[j].mid(2).toLongLong(&ok,16);
#else
            x[j] = parts[j].mid(2).toLong(&ok,16);
#endif
        }
        //qDebug() << "val" << i << x[0] << x[1] << x[2] << x[3];
        regValues[i].setValue(x);
        setRegister(i,regValues[i].value());
    }
    setFontHeightAndWidth(fontHeight,fontWidth);
}

void FloatWindow::contextMenuEvent ( QContextMenuEvent * /* event */ )
{
    QMenu *menu;
    QMenu *single;
    QMenu *all;
    menu   = new QMenu("Float format");
    single = menu->addMenu("Format for 1 register");
    all    = menu->addMenu("Format for all registers");
    single->addAction("float");
    single->addAction("double");
    single->addAction("4 floats");
    single->addAction("2 doubles");
    single->addAction("16 bytes");
    single->addAction("8 shorts");
    single->addAction("4 ints");
    single->addAction("2 longs");
    single->addAction("8 floats");
    single->addAction("4 doubles");
    single->addAction("32 bytes");
    single->addAction("16 shorts");
    single->addAction("8 ints");
    single->addAction("4 longs");
    single->addAction("2 int128s");
    all->addAction("float");
    all->addAction("double");
    all->addAction("4 floats");
    all->addAction("2 doubles");
    all->addAction("16 bytes");
    all->addAction("8 shorts");
    all->addAction("4 ints");
    all->addAction("2 longs");
    all->addAction("8 floats");
    all->addAction("4 doubles");
    all->addAction("32 bytes");
    all->addAction("16 shorts");
    all->addAction("8 ints");
    all->addAction("4 longs");
    all->addAction("2 int128s");
    connect ( single, SIGNAL(triggered(QAction*)),
              this, SLOT(formatRegister(QAction*)) );
    connect ( all, SIGNAL(triggered(QAction*)),
              this, SLOT(formatAllRegisters(QAction*)) );
    menu->exec(QCursor::pos());
    delete menu;
}

void FloatWindow::formatRegister(QAction *action)
{
    int n;

    n = (table->currentColumn()/2) * (count/2) + table->currentRow();
    //qDebug() << n << action->text();
    regValues[n].setFormat(action->text());
    setRegister(n,regValues[n].value());
    setFontHeightAndWidth(fontHeight,fontWidth);
}

void FloatWindow::formatAllRegisters(QAction *action)
{
    for ( int n = 0; n < count; n++ ) {
        //qDebug() << n << action->text();
        regValues[n].setFormat(action->text());
        setRegister(n,regValues[n].value());
    }
    setFontHeightAndWidth(fontHeight,fontWidth);
}

FpRegister::FpRegister()
{
    format = "double";
    for ( int i = 0; i < 4; i++ ) i8[i] = 0;
}

#ifdef Q_WS_WIN
void FpRegister::setValue(unsigned long long *x)
#else
void FpRegister::setValue(unsigned long *x)
#endif
{
    for ( int i = 0; i < 4; i++ ) u8[i] = x[i];
}

QString FpRegister::value()
{
    QString s;
    QString t;
    s.sprintf("%12g",f8[0]);
    if ( format == "float") {
        s.sprintf("%g",f4[0]);
    } else if ( format == "double") {
        s.sprintf("%g",f8[0]);
    } else if ( format == "4 floats") {
        s.sprintf("%g %g %g %g",f4[0],f4[1],f4[2],f4[3]);
    } else if ( format == "2 doubles") {
        s.sprintf("%g %g",f8[0],f8[1]);
    } else if ( format == "16 bytes") {
        s = "";
        for ( int i = 0; i < 16; i++ ) {
            t.sprintf("%02x ",u1[i]);
            s += t;
        }
    } else if ( format == "8 shorts") {
        s = "";
        for ( int i = 0; i < 8; i++ ) {
            t.sprintf("%d ",i2[i]);
            s += t;
        }
    } else if ( format == "4 ints") {
        s.sprintf("%d %d %d %d",i4[0],i4[1],i4[2],i4[3]);
    } else if ( format == "2 longs") {
        s.sprintf("%ld %ld",i8[0],i8[1]);
    } else if ( format == "8 floats") {
        s = "";
        for ( int i = 0; i < 8; i++ ) {
            t.sprintf("%g ",f4[i]);
            s += t;
        }
    } else if ( format == "4 doubles") {
        s = "";
        for ( int i = 0; i < 4; i++ ) {
            t.sprintf("%g ",f8[i]);
            s += t;
        }
    } else if ( format == "32 bytes") {
        s = "";
        for ( int i = 0; i < 32; i++ ) {
            t.sprintf("%02x ",u1[i]);
            s += t;
        }
    } else if ( format == "16 shorts") {
        s = "";
        for ( int i = 0; i < 16; i++ ) {
            t.sprintf("%d ",i2[i]);
            s += t;
        }
    } else if ( format == "8 ints") {
        s = "";
        for ( int i = 0; i < 8; i++ ) {
            t.sprintf("%d ",i4[i]);
            s += t;
        }
    } else if ( format == "4 longs") {
        s.sprintf("%ld %ld %ld %ld",i8[0],i8[1],i8[2],i8[3]);
    } else if ( format == "2 int128s") {
        s.sprintf("%016lx%016lx %016lx%016lx",i8[0],i8[1],i8[2],i8[3]);
    } else {
        qDebug() << "Unknown format" << format;
    }

    return s;
}

void FpRegister::setFormat(QString f)
{
    format = f;
}
