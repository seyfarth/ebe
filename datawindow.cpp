#include "datawindow.h"
#include "settings.h"
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QFrame>
#include <cstdio>


DataWindow::DataWindow(QWidget *parent)
: QFrame(parent)
{
    setObjectName("Data");
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(10,10,10,10);

    table = new QTableWidget(this);
    table->setColumnCount(2);
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    table->setShowGrid(false);

    layout->addWidget(table);

    setLayout ( layout );

    addVariable ( "stack[0-7]", "0x7fffc884888, 0, 0, 12, 0, 1, 2, 15" );
}

QSize DataWindow::sizeHint() const
{
    return QSize(300,100);
}

void DataWindow::setFontHeightAndWidth ( int height, int width )
{
    int max, length;
    int rows = table->rowCount();
    for ( int r = 0; r < rows; r++ ) {
        table->setRowHeight(r,height+3);
    }
    for ( int c = 0; c < 2; c++ ) {
        max = 1;
        for ( int r = 0; r < rows; r++ ) {
            length = table->item(r,c)->text().length();
            if ( length > max ) max = length;
        }
        table->setColumnWidth(c,(max+1)*width+3);
    }
}

void DataWindow::addVariable ( QString name, QString value )
{
    QTableWidgetItem *item;
    names.append(name);
    values.append(value);
    int rows = table->rowCount();
    table->setRowCount(rows+1);
    item = new QTableWidgetItem(name);
    table->setItem(rows,0,item);
    item = new QTableWidgetItem(value);
    table->setItem(rows,1,item);
}
