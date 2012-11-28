#include "datawindow.h"
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QScrollArea>
#include <cstdio>


DataWindow::DataWindow(QWidget *parent)
: QScrollArea(parent)
{
    setObjectName("Data");
    setFrameStyle ( QFrame::Box | QFrame::Raised );
    setLineWidth(5);
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setSpacing(0);
    layout->setContentsMargins(10,10,10,10);

    names = new ListWidget(widget);
    names->setFixedWidth(100);
    values = new ListWidget(widget);
    layout->addWidget(names);
    layout->addWidget(values);

    widget->setLayout ( layout );
    setWidget(widget);
    setWidgetResizable(true);

    addVariable ( "stack[0-7]", "0x7fffc884888, 0, 0, 12, 0, 1, 2, 15" );
}

void DataWindow::setFontWidth ( int width )
{
    int i, n, k, max=1;

    n = names->count();
    for ( i = 0; i < n; i++ ) {
        k = names->item(i)->text().length();
        if ( k > max ) max = k;
    }
    names->setFixedWidth(max*width+8);
}

void DataWindow::addVariable ( QString name, QString value )
{
    names->insertItem ( 0, name+":" );
    values->insertItem ( 0, value );
}
