#include "floatwindow.h"
#include <QHBoxLayout>
#include <cstdio>


FloatWindow::FloatWindow(QWidget *parent)
: QFrame(parent)
{
    setFrameStyle ( QFrame::Box | QFrame::Raised );
    setLineWidth(3);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(10,10,10,10);

    names1 = new QListWidget;
    names1->setStyleSheet("QListWidget{border: 0px}");
    QStringList names;
    names << "xmm0" << "xmm1" << "xmm2" << "xmm3"
          << "xmm4" << "xmm5" << "xmm6" << "xmm7";
    names1->insertItems(0,names);
    names1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    names1->setFixedWidth(55);
    values1 = new QListWidget;
    values1->setStyleSheet("QListWidget{border: 0px}");
    values1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    names.clear();
    names << "0.0" << "0.0" << "0.0" << "0.0"
          << "0.0" << "0.0" << "0.0" << "0.0";
    values1->insertItems(0,names);
    names2 = new QListWidget;
    names2->setStyleSheet("QListWidget{border: 0px}");
    names2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    names2->setFixedWidth(70);
    names.clear();
    names << "xmm8" << "xmm9" << "xmm10" << "xmm11"
          << "xmm12" << "xmm13" << "xmm14" << "xmm15";
    names2->insertItems(0,names);
    values2 = new QListWidget;
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
        
    setLayout ( layout );
}

void FloatWindow::addRegister ( int n, QString value )
{
}
