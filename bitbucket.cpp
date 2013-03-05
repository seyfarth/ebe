#include "bitbucket.h"
#include "stylesheet.h"
#include "settings.h"

#include <iostream>
using namespace std;

BitBucket::BitBucket(QWidget *parent) : QFrame(parent)
{
    setFrameStyle ( QFrame::Panel | QFrame::Raised );
    setLineWidth(4);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(10,10,10,10);

    tab = new QTabWidget;

    unaryPanel = new UnaryBitPanel;
    binaryPanel = new BinaryBitPanel;
    intConvertPanel = new IntConvert;

    tab->addTab ( unaryPanel, "Unary operators" );
    tab->addTab ( binaryPanel, "Binary bit operators" );
    tab->addTab ( intConvertPanel, "Integer conversions" );

    layout->addWidget(tab);

    //connect ( tab, SIGNAL(currentChanged(int)), this, SLOT(changedTab(int)) );

    setLayout(layout);

}

void BitBucket::setFontHeightAndWidth ( int height, int width )
{
    fontHeight = height;
    fontWidth = width;
}

