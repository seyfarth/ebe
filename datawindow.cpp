#include "datawindow.h"
#include <QHBoxLayout>
#include <QLabel>


DataWindow::DataWindow(QWidget *parent)
: QFrame(parent)
{
    setFrameStyle ( QFrame::Box | QFrame::Raised );
    setLineWidth(3);
    dataLayout = new QVBoxLayout(this);
    dataLayout->setSpacing(2);
    dataLayout->setContentsMargins(3,3,3,3);
    addVariable ( "stack[0-7]", "0x7fffc884888, 0, 0, 12, 0, 1, 2, 15" );
    setLayout ( dataLayout );
}

void DataWindow::addVariable ( QString name, QString value )
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *label = new QLabel(name,this);
    layout->addWidget(label);
    label = new QLabel(value,this);
    layout->addWidget(label);
    dataLayout->addLayout(layout);
}
