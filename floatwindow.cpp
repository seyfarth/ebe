#include "floatwindow.h"
#include <QHBoxLayout>
#include <QLabel>


FloatWindow::FloatWindow(QWidget *parent)
: QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(6);
    floatLayout = new QVBoxLayout(this);
    floatLayout->setSpacing(2);
    floatLayout->setContentsMargins(3,3,3,3);
    for ( int i = 0; i < 16; i++ ) {
        floatLabels[i] = new QLabel ( "0.0" );
    }

    for ( int row = 0; row < 8; row++ ) {
        char name[10];
        QHBoxLayout *layout = new QHBoxLayout(this);
        for ( int col = 0; col < 2; col++ ) {
            sprintf(name,"xmm%d",row+col*8);
            QLabel *label = new QLabel(name,this);
            layout->addWidget(label);
            layout->addWidget(floatLabels[row+col*8]);
        }
        floatLayout->addLayout(layout);
    }
        
    setLayout ( floatLayout );
}

void FloatWindow::addRegister ( int n, QString value )
{
    if ( n >= 0 && n < 16 ) {
        floatLabels[n]->setText(value);
    }
}
