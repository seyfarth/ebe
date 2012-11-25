#include "floatwindow.h"
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QScrollArea>
#include <cstdio>


FloatWindow::FloatWindow(QWidget *parent)
: QScrollArea(parent)
{
    setFrameStyle ( QFrame::Box | QFrame::Raised );
    setLineWidth(5);
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setSpacing(0);
    layout->setContentsMargins(10,10,10,10);

    names1 = new QListWidget(widget);
    names1->setStyleSheet("QListWidget{border: 0px}");
    QStringList names;
    names << "xmm0" << "xmm1" << "xmm2" << "xmm3"
          << "xmm4" << "xmm5" << "xmm6" << "xmm7";
    names1->insertItems(0,names);
    names1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    names1->setFixedWidth(55);
    values1 = new QListWidget(widget);
    values1->setStyleSheet("QListWidget{border: 0px}");
    values1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    names.clear();
    names << "0.0" << "0.0" << "0.0" << "0.0"
          << "0.0" << "0.0" << "0.0" << "0.0";
    values1->insertItems(0,names);
    names2 = new QListWidget(widget);
    names2->setStyleSheet("QListWidget{border: 0px}");
    names2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    names2->setFixedWidth(70);
    names.clear();
    names << "xmm8" << "xmm9" << "xmm10" << "xmm11"
          << "xmm12" << "xmm13" << "xmm14" << "xmm15";
    names2->insertItems(0,names);
    values2 = new QListWidget(widget);
    values2->setStyleSheet("QListWidget{border: 0px}");
    values2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    names.clear();
    names << "0.0" << "0.0" << "0.0" << "0.0"
          << "0.0" << "0.0" << "0.0" << "0.0";
    values2->insertItems(0,names);

    layout->addWidget(names1);
    layout->addWidget(values1);
    layout->addWidget(names2);
    layout->addWidget(values2);
        
    for ( int i = 0; i < 8; i++ ) {
        regs[i] = values1->item(i);
        regs[i+8] = values2->item(i);
    }

    widget->setLayout ( layout );
    setWidget(widget);
    setWidgetResizable(true);

    setRegister(4,"4.0");
    setRegister(8,"8.0");
    setRegister(15,"15.0");
}

void FloatWindow::setRegister ( int n, QString value )
{
    if ( n >= 0 && n < 16 ) regs[n]->setText(value);
}
