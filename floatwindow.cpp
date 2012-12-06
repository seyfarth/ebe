#include "floatwindow.h"
#include "settings.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
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
            regs[c*2+r] = value;
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
